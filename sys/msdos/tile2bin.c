/*   SCCS Id: @(#)tile2bin.c   3.2     95/01/26                     */
/*   Copyright (c) NetHack PC Development Team 1993, 1994, 1995     */
/*   NetHack may be freely redistributed.  See license for details. */

/*
 * Edit History:
 *
 *	Initial Creation			M.Allison	93/10/21
 *	ifndef MONITOR_HEAP for heaputil.c	P.Winner	94/03/12
 *      added Borland C _stklen variable	Y.Sapir		94/05/01
 *	fixed to use text tiles from win/share	M.Allison	95/01/31
 *
 */


#include "hack.h"
#include "pcvideo.h"
#include "tile.h"
#include "pctiles.h"

#include <dos.h>
#ifndef MONITOR_HEAP
#include <stdlib.h>
#endif
#include <time.h>

#ifdef __GO32__
#include <unistd.h>
#endif

#if defined(_MSC_VER) && _MSC_VER >= 700
#pragma warning(disable:4309)	/* initializing */
#pragma warning(disable:4018)	/* signed/unsigned mismatch */
#pragma warning(disable:4131)	/* old style declarator */
#pragma warning(disable:4127)	/* conditional express. is constant */
#endif

#ifdef __BORLANDC__
extern unsigned _stklen = STKSIZ;
#endif

extern char *FDECL(tilename, (int, int));

#ifdef PLANAR_FILE
char masktable[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
char charcolors[MAXCOLORMAPSIZE];
#ifdef OVERVIEW_FILE
struct overview_planar_cell_struct planetile;
#else
struct planar_cell_struct planetile;
#endif
FILE *tibfile1;
#endif

#ifdef PACKED_FILE
char packtile[TILE_Y][TILE_X];
FILE *tibfile2;
#endif

int num_colors;
pixel pixels[TILE_Y][TILE_X];
struct tibhdr_struct tibheader;

static void FDECL(write_tibtile, (int));
static void FDECL(write_tibheader, (FILE *, struct tibhdr_struct *));
static void FDECL(build_tibtile,  (pixel (*)[TILE_X]));

#ifndef OVERVIEW_FILE
char *tilefiles[] = {	"../win/share/monsters.txt",
			"../win/share/objects.txt",
			"../win/share/other.txt"};
#else
char *tilefiles[] = {	"../win/share/monthin.txt",
			"../win/share/objthin.txt",
			"../win/share/oththin.txt"};
#endif

int tilecount;
int filenum;
int paletteflag;

int
main(argc, argv)
int argc;
char *argv[];
{
	int i;
	struct tm *newtime;
	time_t aclock;
	char *paletteptr;

	if (argc != 1) {
		Fprintf(stderr, "usage: tile2bin (from the util directory)\n");
		exit(EXIT_FAILURE);
	}

#ifdef PLANAR_FILE
# ifndef OVERVIEW_FILE
	tibfile1 = fopen(NETHACK_PLANAR_TILEFILE, WRBMODE);
# else
	tibfile1 = fopen(NETHACK_OVERVIEW_TILEFILE, WRBMODE);
# endif
	if (tibfile1 == (FILE *)0) {
		Fprintf(stderr, "Unable to open output file %s\n",
# ifndef OVERVIEW_FILE
				NETHACK_PLANAR_TILEFILE);
#else
				NETHACK_OVERVIEW_TILEFILE);
#endif
		exit(EXIT_FAILURE);
	}
#endif

#ifdef PACKED_FILE
	tibfile2 = fopen(NETHACK_PACKED_TILEFILE, WRBMODE);
	if (tibfile2 == (FILE *)0) {
		Fprintf(stderr, "Unable to open output file %s\n",
				NETHACK_PACKED_TILEFILE);
		exit(EXIT_FAILURE);
	}
#endif
	time(&aclock);
	newtime = localtime(&aclock);

	tilecount = 0;
	paletteflag = 0;	
	filenum = 0;
	while (filenum < 3) {
		if (!fopen_text_file(tilefiles[filenum], RDTMODE)) {
			Fprintf(stderr,
			 "usage: tile2bin (from the util or src directory)\n");
			exit(EXIT_FAILURE);
		}
		num_colors = colorsinmap;
		if (num_colors > 62) {
			Fprintf(stderr, "too many colors (%d)\n", num_colors);
			exit(EXIT_FAILURE);
		}

		if (!paletteflag) {
			paletteptr = tibheader.palette;
			for (i = 0; i < num_colors; i++) {
				*paletteptr++ =	ColorMap[CM_RED][i],
				*paletteptr++ = ColorMap[CM_GREEN][i],
				*paletteptr++ = ColorMap[CM_BLUE][i];
			}
			paletteflag++;
		}

				
		while (read_text_tile(pixels)) {
			build_tibtile(pixels);
			write_tibtile(tilecount);
			tilecount++;
		}

		(void) fclose_text_file();
		++filenum;
	}

#  if defined(_MSC_VER)
	tibheader.compiler = MSC_COMP;
#  elif defined(__BORLANDC__)
	tibheader.compiler = BC_COMP;
#  elif defined(__GO32__)
	tibheader.compiler = DJGPP_COMP;
#  else
	tibheader.compiler = OTHER_COMP;
#  endif
	
	strncpy(tibheader.ident,
		"NetHack 3.2 MSDOS Port binary tile file", 80);
	strncpy(tibheader.timestamp, asctime(newtime), 24);
	tibheader.timestamp[25] = '\0';
	tibheader.tilecount = tilecount;
	tibheader.numcolors = num_colors;
# ifdef PLANAR_FILE
	tibheader.tilestyle = PLANAR_STYLE;
	write_tibheader(tibfile1, &tibheader);
	(void) fclose(tibfile1);
#  ifndef OVERVIEW_FILE
	Fprintf(stderr, "Total of %d planar tiles written to %s.\n", 
		tilecount, NETHACK_PLANAR_TILEFILE);
#  else
	Fprintf(stderr, "Total of %d planar tiles written to %s.\n", 
		tilecount, NETHACK_OVERVIEW_TILEFILE);
#  endif
# endif

# ifdef PACKED_FILE
	tibheader.tilestyle = PACKED_STYLE;
	write_tibheader(tibfile2, &tibheader);
	Fprintf(stderr, "Total of %d packed tiles written to %s.\n", 
		tilecount, NETHACK_PACKED_TILEFILE);
	(void) fclose(tibfile2);
# endif

	exit(EXIT_SUCCESS);
	/*NOTREACHED*/
	return 0;
}


static void
write_tibheader(fileptr,tibhdr)
FILE *fileptr;
struct tibhdr_struct *tibhdr;
{

	if (fseek(fileptr,0L,SEEK_SET)) {
		Fprintf(stderr, "Error writing header to tile file\n");
	}
	fwrite(tibhdr, sizeof(struct tibhdr_struct), 1, fileptr);
}

static void
build_tibtile(pixels)
pixel (*pixels)[TILE_X];
{
	int i, j, k, co_off;
	unsigned char co_mask,tmp;

#ifndef OVERVIEW_FILE
	memset((void *)&planetile,0,sizeof(struct planar_cell_struct));
#else
	memset((void *)&planetile,0,
		sizeof(struct overview_planar_cell_struct));
#endif
	for (j = 0; j < TILE_Y; j++) {
		for (i = 0; i < TILE_X; i++) {
			for (k = 0; k < num_colors; k++) {				
				if (ColorMap[CM_RED][k] == pixels[j][i].r &&
				    ColorMap[CM_GREEN][k] == pixels[j][i].g &&
				    ColorMap[CM_BLUE][k] == pixels[j][i].b)
					break;
			}
			if (k >= num_colors)
				Fprintf(stderr, "color not in colormap!\n");
#ifdef PACKED_FILE
			packtile[j][i] = k;
#endif

#ifdef PLANAR_FILE
			if (i > 7) {
				co_off = 1;
				co_mask = masktable[i - 8];
			} else {
				co_off = 0;
				co_mask = masktable[i];
			}

			tmp  = planetile.plane[0].image[j][co_off];
			planetile.plane[0].image[j][co_off] = (k & 0x0008) ?
				(tmp | co_mask) :
				(tmp & ~co_mask);

			tmp  = planetile.plane[1].image[j][co_off];
			planetile.plane[1].image[j][co_off] = (k & 0x0004) ?
				(tmp | co_mask) :
				(tmp & ~co_mask);

			tmp  = planetile.plane[2].image[j][co_off];
			planetile.plane[2].image[j][co_off] = (k & 0x0002) ?
				(tmp | co_mask) :
				(tmp & ~co_mask);

			tmp  = planetile.plane[3].image[j][co_off];
			planetile.plane[3].image[j][co_off] = (k & 0x0001) ?
				(tmp | co_mask) :
				(tmp & ~co_mask);
#endif /* PLANAR_FILE */
		}
	}
}

static void
write_tibtile(recnum)
int recnum;
{
	long fpos;

#ifdef PLANAR_FILE
# ifndef OVERVIEW_FILE
	fpos = ((long)(recnum) * (long)sizeof(struct planar_cell_struct)) +
		(long)TIBHEADER_SIZE;
# else
	fpos = ((long)(recnum) * 
			(long)sizeof(struct overview_planar_cell_struct)) +
			(long)TIBHEADER_SIZE;
# endif
	if (fseek(tibfile1,fpos,SEEK_SET)) {
		Fprintf(stderr, "Error seeking before planar tile write %d\n",
			recnum);
	}
# ifndef OVERVIEW_FILE
	fwrite(&planetile, sizeof(struct planar_cell_struct), 1, tibfile1);
# else
	fwrite(&planetile,
		sizeof(struct overview_planar_cell_struct), 1, tibfile1);
# endif
#endif

#ifdef PACKED_FILE
	fpos = ((long)(recnum) * (long)sizeof(packtile)) +
		(long)TIBHEADER_SIZE;
	if (fseek(tibfile2,fpos,SEEK_SET)) {
		Fprintf(stderr, "Error seeking before packed tile write %d\n",
			recnum);
	}
	fwrite(&packtile, sizeof(packtile), 1, tibfile2);
#endif
}
