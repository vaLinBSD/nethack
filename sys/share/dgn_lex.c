/* A lexical scanner generated by flex */

/* scanner skeleton version:
 * $Header: /usr/fsys/odin/a/vern/flex/RCS/flex.skel,v 2.16 90/08/03 14:09:36 vern Exp $
 */

#define FLEX_SCANNER

#include <stdio.h>


/* cfront 1.2 defines "c_plusplus" instead of "__cplusplus" */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif


#ifdef __cplusplus

#include <stdlib.h>
#include <osfcn.h>

/* use prototypes in function declarations */
#define YY_USE_PROTOS

/* the "const" storage-class-modifier is valid */
#define YY_USE_CONST

#else	/* ! __cplusplus */

#ifdef __STDC__

#ifdef __GNUC__
#include <stddef.h>
void *malloc( size_t );
#else
#include <stdlib.h>
#endif	/* __GNUC__ */

#define YY_USE_PROTOS
#define YY_USE_CONST

#endif	/* __STDC__ */
#endif	/* ! __cplusplus */


#ifdef __TURBOC__
#define YY_USE_CONST
#endif
#ifdef VMS
# define YY_USE_CONST
# ifdef USE_PROTOTYPES
#  define YY_USE_PROTOS
# endif
#endif


#ifndef YY_USE_CONST
#define const
#endif


#ifdef YY_USE_PROTOS
#define YY_PROTO(proto) proto
#else
#define YY_PROTO(proto) ()
/* we can't get here if it's an ANSI C compiler, or a C++ compiler,
 * so it's got to be a K&R compiler, and therefore there's no standard
 * place from which to include these definitions
 */
char *malloc();
int read();
#endif


/* amount of stuff to slurp up with each read */
#ifndef YY_READ_BUF_SIZE
#define YY_READ_BUF_SIZE 8192
#endif

/* returned upon end-of-file */
#define YY_END_TOK 0

/* copy whatever the last rule matched to the standard output */

/* cast to (char *) is because for 8-bit chars, yytext is (unsigned char *) */
/* this used to be an fputs(), but since the string might contain NUL's,
 * we now use fwrite()
 */
#define ECHO (void) fwrite( (char *) yytext, yyleng, 1, yyout )

/* gets input and stuffs it into "buf".  number of characters read, or YY_NULL,
 * is returned in "result".
 */
#define YY_INPUT(buf,result,max_size) \
	if ( (result = read( fileno(yyin), (char *) buf, max_size )) < 0 ) \
	    YY_FATAL_ERROR( "read() in flex scanner failed" );
#define YY_NULL 0

/* no semi-colon after return; correct usage is to write "yyterminate();" -
 * we don't want an extra ';' after the "return" because that will cause
 * some compilers to complain about unreachable statements.
 */
#define yyterminate() return ( YY_NULL )

/* report a fatal error */

/* The funky do-while is used to turn this macro definition into
 * a single C statement (which needs a semi-colon terminator).
 * This avoids problems with code like:
 *
 * 	if ( something_happens )
 *		YY_FATAL_ERROR( "oops, the something happened" );
 *	else
 *		everything_okay();
 *
 * Prior to using the do-while the compiler would get upset at the
 * "else" because it interpreted the "if" statement as being all
 * done when it reached the ';' after the YY_FATAL_ERROR() call.
 */

#define YY_FATAL_ERROR(msg) \
	do \
		{ \
		(void) fputs( msg, stderr ); \
		(void) putc( '\n', stderr ); \
		exit( 1 ); \
		} \
	while ( 0 )

/* default yywrap function - always treat EOF as an EOF */
#define yywrap() 1

/* enter a start condition.  This macro really ought to take a parameter,
 * but we do it the disgusting crufty way forced on us by the ()-less
 * definition of BEGIN
 */
#define BEGIN yy_start = 1 + 2 *

/* action number for EOF rule of a given start state */
#define YY_STATE_EOF(state) (YY_END_OF_BUFFER + state + 1)

/* special action meaning "start processing a new file" */
#define YY_NEW_FILE \
	do \
		{ \
		yy_init_buffer( yy_current_buffer, yyin ); \
		yy_load_buffer_state(); \
		} \
	while ( 0 )

/* default declaration of generated scanner - a define so the user can
 * easily add parameters
 */
#define YY_DECL int yylex YY_PROTO(( void )) 

/* code executed at the end of each rule */
#define YY_BREAK break;

#define YY_END_OF_BUFFER_CHAR 0

#ifndef YY_BUF_SIZE
#define YY_BUF_SIZE (YY_READ_BUF_SIZE * 2) /* size of default input buffer */
#endif

typedef struct yy_buffer_state *YY_BUFFER_STATE;

#define YY_CHAR char
# line 1 "dgn_comp.l"
#define INITIAL 0
# line 2 "dgn_comp.l"
/*	SCCS Id: @(#)dgn_lex.c	3.1	92/10/23	*/
/*	Copyright (c) 1989 by Jean-Christophe Collet */
/*	Copyright (c) 1990 by M. Stephenson	     */
/* NetHack may be freely redistributed.  See license for details. */

#define DGN_COMP

#include "config.h"
#include "dgn_comp.h"
#include "dgn_file.h"

long *FDECL(alloc, (unsigned int));
/*
 * Most of these don't exist in flex, yywrap is macro and
 * yyunput is properly declared in flex.skel.
 */
#ifndef FLEX_SCANNER
int FDECL (yyback, (int *, int));
int NDECL (yylook);
int NDECL (yyinput);
int NDECL (yywrap);
int NDECL (yylex);
	/* Traditional lexes let yyunput() and yyoutput() default to int;
	 * newer ones may declare them as void since they don't return
	 * values.  For even more fun, the lex supplied as part of the
	 * newer unbundled compiler for SunOS 4.x adds the void declarations
	 * (under __STDC__ or _cplusplus ifdefs -- otherwise they remain
	 * int) while the bundled lex and the one with the older unbundled
	 * compiler do not.  To detect this, we need help from outside --
	 * sys/unix/Makefile.utl.
	 */
# if defined(NeXT) || defined(SVR4)
#  define VOIDYYPUT
# endif
# if !defined(VOIDYYPUT)
#  if defined(POSIX_TYPES) && !defined(BOS) && !defined(HISX)
#   define VOIDYYPUT
#  endif
# endif
# if !defined(VOIDYYPUT) && defined(WEIRD_LEX)
#  if defined(SUNOS4) && defined(__STDC__) && (WEIRD_LEX > 1) 
#   define VOIDYYPUT
#  endif
# endif
# ifdef VOIDYYPUT
void FDECL (yyunput, (int));
void FDECL (yyoutput, (int));
# else
int FDECL (yyunput, (int));
int FDECL (yyoutput, (int));
# endif
#endif	/* FLEX_SCANNER */

void FDECL (init_yyin, (FILE *));
void FDECL (init_yyout, (FILE *));

#ifdef MICRO
#undef exit
extern void FDECL(exit, (int));
#endif

/* this doesn't always get put in dgn_comp.h
 * (esp. when using older versions of bison)
 */

extern YYSTYPE yylval;

int line_number = 1;
/*
 *	This is a hack required by Michael Hamel to get things
 *	working on the Mac.
 */
#if defined(applec) && !defined(FLEX_SCANNER)
#undef input
#undef unput
#define unput(c) { yytchar = (c); if (yytchar == 10) yylineno--; *yysptr++ = yytchar; }				  
# ifndef YYNEWLINE
# define YYNEWLINE 10
# endif

char
input() {	/* Under MPW \n is chr(13)! Compensate for this. */

	if (yysptr > yysbuf) return(*--yysptr);
	else {
		yytchar = getc(yyin);
	 	if (yytchar == '\n') {
		    yylineno++;
		    return(YYNEWLINE);
		}
		if (yytchar == EOF) return(0);
		else		    return(yytchar);
	}
}
#endif	/* applec && !FLEX_SCANNER */

# line 99 "dgn_comp.l"

/* done after the current pattern has been matched and before the
 * corresponding action - sets up yytext
 */
#define YY_DO_BEFORE_ACTION \
	yytext = yy_bp; \
	yyleng = yy_cp - yy_bp; \
	yy_hold_char = *yy_cp; \
	*yy_cp = '\0'; \
	yy_c_buf_p = yy_cp;

#define EOB_ACT_CONTINUE_SCAN 0
#define EOB_ACT_END_OF_FILE 1
#define EOB_ACT_LAST_MATCH 2

/* return all but the first 'n' matched characters back to the input stream */
#define yyless(n) \
	do \
		{ \
		/* undo effects of setting up yytext */ \
		*yy_cp = yy_hold_char; \
		yy_c_buf_p = yy_cp = yy_bp + n; \
		YY_DO_BEFORE_ACTION; /* set up yytext again */ \
		} \
	while ( 0 )

#define unput(c) yyunput( c, yytext )


struct yy_buffer_state
    {
    FILE *yy_input_file;

    YY_CHAR *yy_ch_buf;		/* input buffer */
    YY_CHAR *yy_buf_pos;	/* current position in input buffer */

    /* size of input buffer in bytes, not including room for EOB characters*/
    int yy_buf_size;	

    /* number of characters read into yy_ch_buf, not including EOB characters */
    int yy_n_chars;

    int yy_eof_status;		/* whether we've seen an EOF on this buffer */
#define EOF_NOT_SEEN 0
    /* "pending" happens when the EOF has been seen but there's still
     * some text process
     */
#define EOF_PENDING 1
#define EOF_DONE 2
    };

static YY_BUFFER_STATE yy_current_buffer;

/* we provide macros for accessing buffer states in case in the
 * future we want to put the buffer states in a more general
 * "scanner state"
 */
#define YY_CURRENT_BUFFER yy_current_buffer


/* yy_hold_char holds the character lost when yytext is formed */
static YY_CHAR yy_hold_char;

static int yy_n_chars;		/* number of characters read into yy_ch_buf */



#ifndef YY_USER_ACTION
#define YY_USER_ACTION
#endif

#ifndef YY_USER_INIT
#define YY_USER_INIT
#endif

extern YY_CHAR *yytext;
extern int yyleng;
extern FILE *yyin, *yyout;

YY_CHAR *yytext;
int yyleng;

FILE *yyin = (FILE *) 0, *yyout = (FILE *) 0;

#define YY_END_OF_BUFFER 36
typedef int yy_state_type;
static const short int yy_accept[194] =
    {   0,
        0,    0,   36,   34,   33,   32,   34,   29,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   33,    0,
       30,   29,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    2,    0,   31,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        3,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,   14,    0,    0,    0,    0,    0,    0,    4,    0,
       25,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        6,    0,    0,    0,    5,    0,    0,   23,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,   20,
        0,    0,    0,    0,    8,    0,    0,    0,    0,    0,
        0,    1,    0,    0,    0,    0,    0,   22,   15,    0,
       21,    7,   19,    0,    0,    0,    0,    0,    0,   13,
        0,    0,    0,   26,   16,    0,    0,   12,    0,    0,
        0,   11,    9,    0,   17,   18,    0,   27,    0,   28,
       24,   10,    0

    } ;

static const YY_CHAR yy_ec[128] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    2,    1,    4,    5,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    6,    1,    1,    6,    6,    6,
        6,    6,    6,    6,    6,    6,    6,    1,    1,    1,
        1,    1,    1,    1,    7,    8,    9,   10,   11,   12,
       13,   14,   15,    1,    1,   16,   17,   18,   19,   20,
        1,   21,   22,   23,   24,   25,    1,    1,   26,    1,
        1,    1,    1,    1,   27,    1,   28,    1,   29,   30,

       31,   32,   33,   34,   35,    1,   36,   37,   38,   39,
       40,   41,    1,   42,   43,   44,   45,    1,   46,    1,
        1,   47,    1,    1,    1,    1,    1
    } ;

static const YY_CHAR yy_meta[48] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1
    } ;

static const short int yy_base[196] =
    {   0,
        0,  212,  216,  218,  213,  218,  210,  207,  196,  190,
      196,   37,  191,  197,  186,  188,  171,  164,  172,  174,
      173,   18,  160,  159,  154,  157,   11,  193,  193,  190,
      218,  187,  177,  184,  183,  167,  170,  164,  161,  166,
      174,  155,  136,  144,  134,  132,  133,   26,  135,  143,
      147,  128,  145,  218,  169,  218,  158,  152,  154,  159,
      154,  145,   44,  142,   47,  124,  124,  125,  129,  129,
      115,   27,  121,  113,  111,  120,  115,  116,  134,  142,
      132,  128,  137,  121,  130,  129,  125,  129,  131,   97,
      218,  105,   94,  101,   95,   96,   94,   99,  105,  101,

       89,  218,   95,  112,  114,   51,  112,  107,  218,  110,
      114,  111,  106,   96,   85,   76,   81,   82,   88,   69,
      218,   81,   76,   75,  218,   78,   99,  218,   88,   97,
       87,   88,   92,   93,   88,   91,   90,   71,   65,  218,
       62,   60,   57,   56,  218,   59,   54,   74,   84,   65,
       66,  218,   70,   65,   70,   60,   68,  218,  218,   52,
      218,  218,  218,   46,   50,   57,   61,   67,   62,  218,
       67,   64,   63,  218,  218,   42,   41,  218,   61,   53,
       49,  218,  218,   50,  218,  218,   51,  218,   46,  218,
      218,  218,  218,   61,   59

    } ;

static const short int yy_def[196] =
    {   0,
      193,    1,  193,  193,  193,  193,  194,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  195,  193,  194,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  195,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,

      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,    0,  193,  193

    } ;

static const short int yy_nxt[266] =
    {   0,
        4,    5,    6,    7,    4,    8,    9,   10,   11,   12,
       13,    4,    4,    4,    4,   14,    4,    4,    4,   15,
       16,    4,    4,    4,    4,    4,    4,    4,   17,   18,
        4,    4,    4,   19,    4,    4,   20,   21,   22,    4,
       23,   24,   25,   26,   27,    4,    4,   36,   47,   53,
       85,   54,   72,   73,   86,   88,   96,   48,  129,   55,
       37,   30,   89,  192,  191,  190,  130,  189,  188,  187,
      186,   97,  185,  184,  183,  182,  181,  180,  179,  178,
      177,  176,  175,  174,  173,  172,  171,  170,  169,  168,
      167,  166,  165,  164,  163,  162,  161,  160,  159,  158,

      157,  156,  155,  154,  153,  152,  151,  150,  149,  148,
      147,  146,  145,  144,  143,  142,  141,  140,  139,  138,
      137,  136,  135,  134,  133,  132,  131,  128,  127,  126,
      125,  124,  123,  122,  121,  120,  119,  118,  117,  116,
      115,  114,  113,  112,  111,  110,  109,  108,  107,  106,
      105,  104,  103,  102,  101,  100,   99,   98,   95,   94,
       93,   92,   91,   90,   87,   84,   83,   82,   81,   80,
       79,   56,   78,   77,   76,   75,   74,   71,   70,   69,
       68,   67,   66,   65,   64,   63,   62,   61,   60,   59,
       58,   57,   32,   31,   29,   56,   52,   51,   50,   49,

       46,   45,   44,   43,   42,   41,   40,   39,   38,   35,
       34,   33,   32,   31,   29,  193,   28,    3,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193
    } ;

static const short int yy_chk[266] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,   12,   22,   27,
       63,   27,   48,   48,   63,   65,   72,   22,  106,  195,
       12,  194,   65,  189,  187,  184,  106,  181,  180,  179,
      177,   72,  176,  173,  172,  171,  169,  168,  167,  166,
      165,  164,  160,  157,  156,  155,  154,  153,  151,  150,
      149,  148,  147,  146,  144,  143,  142,  141,  139,  138,

      137,  136,  135,  134,  133,  132,  131,  130,  129,  127,
      126,  124,  123,  122,  120,  119,  118,  117,  116,  115,
      114,  113,  112,  111,  110,  108,  107,  105,  104,  103,
      101,  100,   99,   98,   97,   96,   95,   94,   93,   92,
       90,   89,   88,   87,   86,   85,   84,   83,   82,   81,
       80,   79,   78,   77,   76,   75,   74,   73,   71,   70,
       69,   68,   67,   66,   64,   62,   61,   60,   59,   58,
       57,   55,   53,   52,   51,   50,   49,   47,   46,   45,
       44,   43,   42,   41,   40,   39,   38,   37,   36,   35,
       34,   33,   32,   30,   29,   28,   26,   25,   24,   23,

       21,   20,   19,   18,   17,   16,   15,   14,   13,   11,
       10,    9,    8,    7,    5,    3,    2,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
      193,  193,  193,  193,  193
    } ;

static yy_state_type yy_last_accepting_state;
static YY_CHAR *yy_last_accepting_cpos;

/* the intent behind this definition is that it'll catch
 * any uses of REJECT which flex missed
 */
#define REJECT reject_used_but_not_detected
#define yymore() yymore_used_but_not_detected
#define YY_MORE_ADJ 0

/* these variables are all declared out here so that section 3 code can
 * manipulate them
 */
/* points to current character in buffer */
static YY_CHAR *yy_c_buf_p = (YY_CHAR *) 0;
static int yy_init = 1;		/* whether we need to initialize */
static int yy_start = 0;	/* start state number */

/* flag which is used to allow yywrap()'s to do buffer switches
 * instead of setting up a fresh yyin.  A bit of a hack ...
 */
static int yy_did_buffer_switch_on_eof;

static yy_state_type yy_get_previous_state YY_PROTO(( void ));
static yy_state_type yy_try_NUL_trans YY_PROTO(( yy_state_type current_state ));
static int yy_get_next_buffer YY_PROTO(( void ));
static void yyunput YY_PROTO(( YY_CHAR c, YY_CHAR *buf_ptr ));
void yyrestart YY_PROTO(( FILE *input_file ));
void yy_switch_to_buffer YY_PROTO(( YY_BUFFER_STATE new_buffer ));
void yy_load_buffer_state YY_PROTO(( void ));
YY_BUFFER_STATE yy_create_buffer YY_PROTO(( FILE *file, int size ));
void yy_delete_buffer YY_PROTO(( YY_BUFFER_STATE b ));
void yy_init_buffer YY_PROTO(( YY_BUFFER_STATE b, FILE *file ));

#define yy_new_buffer yy_create_buffer

#ifdef __cplusplus
static int yyinput YY_PROTO(( void ));
#else
static int input YY_PROTO(( void ));
#endif

YY_DECL
    {
    register yy_state_type yy_current_state;
    register YY_CHAR *yy_cp, *yy_bp;
    register int yy_act;



    if ( yy_init )
	{
	YY_USER_INIT;

	if ( ! yy_start )
	    yy_start = 1;	/* first start state */

	if ( ! yyin )
	    yyin = stdin;

	if ( ! yyout )
	    yyout = stdout;

	if ( yy_current_buffer )
	    yy_init_buffer( yy_current_buffer, yyin );
	else
	    yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE );

	yy_load_buffer_state();

	yy_init = 0;
	}

    while ( 1 )		/* loops until end-of-file is reached */
	{
	yy_cp = yy_c_buf_p;

	/* support of yytext */
	*yy_cp = yy_hold_char;

	/* yy_bp points to the position in yy_ch_buf of the start of the
	 * current run.
	 */
	yy_bp = yy_cp;

	yy_current_state = yy_start;
	if ( yy_bp[-1] == '\n' )
	    ++yy_current_state;
yy_match:
	do
	    {
	    register YY_CHAR yy_c = yy_ec[*yy_cp];
	    if ( yy_accept[yy_current_state] )
		{
		yy_last_accepting_state = yy_current_state;
		yy_last_accepting_cpos = yy_cp;
		}
	    while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
		{
		yy_current_state = yy_def[yy_current_state];
		if ( yy_current_state >= 194 )
		    yy_c = yy_meta[yy_c];
		}
	    yy_current_state = yy_nxt[yy_base[yy_current_state] + yy_c];
	    ++yy_cp;
	    }
	while ( yy_current_state != 193 );
	yy_cp = yy_last_accepting_cpos;
	yy_current_state = yy_last_accepting_state;

yy_find_action:
	yy_act = yy_accept[yy_current_state];

	YY_DO_BEFORE_ACTION;
	YY_USER_ACTION;

do_action:	/* this label is used only to access EOF actions */


	switch ( yy_act )
	    {
	    case 0: /* must backtrack */
	    /* undo the effects of YY_DO_BEFORE_ACTION */
	    *yy_cp = yy_hold_char;
	    yy_cp = yy_last_accepting_cpos;
	    yy_current_state = yy_last_accepting_state;
	    goto yy_find_action;

case 1:
# line 100 "dgn_comp.l"
return(A_DUNGEON);
	YY_BREAK
case 2:
# line 101 "dgn_comp.l"
{ yylval.i=1; return(UP_OR_DOWN); }
	YY_BREAK
case 3:
# line 102 "dgn_comp.l"
{ yylval.i=0; return(UP_OR_DOWN); }
	YY_BREAK
case 4:
# line 103 "dgn_comp.l"
return(ENTRY);
	YY_BREAK
case 5:
# line 104 "dgn_comp.l"
return(STAIR);
	YY_BREAK
case 6:
# line 105 "dgn_comp.l"
return(NO_UP);
	YY_BREAK
case 7:
# line 106 "dgn_comp.l"
return(NO_DOWN);
	YY_BREAK
case 8:
# line 107 "dgn_comp.l"
return(PORTAL);
	YY_BREAK
case 9:
# line 108 "dgn_comp.l"
return(PROTOFILE);
	YY_BREAK
case 10:
# line 109 "dgn_comp.l"
return(DESCRIPTION);
	YY_BREAK
case 11:
# line 110 "dgn_comp.l"
return(LEVELDESC);
	YY_BREAK
case 12:
# line 111 "dgn_comp.l"
return(ALIGNMENT);
	YY_BREAK
case 13:
# line 112 "dgn_comp.l"
return(LEVALIGN);
	YY_BREAK
case 14:
# line 113 "dgn_comp.l"
{ yylval.i=TOWN ; return(DESCRIPTOR); }
	YY_BREAK
case 15:
# line 114 "dgn_comp.l"
{ yylval.i=HELLISH ; return(DESCRIPTOR); }
	YY_BREAK
case 16:
# line 115 "dgn_comp.l"
{ yylval.i=MAZELIKE ; return(DESCRIPTOR); }
	YY_BREAK
case 17:
# line 116 "dgn_comp.l"
{ yylval.i=ROGUELIKE ; return(DESCRIPTOR); }
	YY_BREAK
case 18:
# line 117 "dgn_comp.l"
{ yylval.i=D_ALIGN_NONE ; return(DESCRIPTOR); }
	YY_BREAK
case 19:
# line 118 "dgn_comp.l"
{ yylval.i=D_ALIGN_NONE ; return(DESCRIPTOR); }
	YY_BREAK
case 20:
# line 119 "dgn_comp.l"
{ yylval.i=D_ALIGN_LAWFUL ; return(DESCRIPTOR); }
	YY_BREAK
case 21:
# line 120 "dgn_comp.l"
{ yylval.i=D_ALIGN_NEUTRAL ; return(DESCRIPTOR); }
	YY_BREAK
case 22:
# line 121 "dgn_comp.l"
{ yylval.i=D_ALIGN_CHAOTIC ; return(DESCRIPTOR); }
	YY_BREAK
case 23:
# line 122 "dgn_comp.l"
return(BRANCH);
	YY_BREAK
case 24:
# line 123 "dgn_comp.l"
return(CHBRANCH);
	YY_BREAK
case 25:
# line 124 "dgn_comp.l"
return(LEVEL);
	YY_BREAK
case 26:
# line 125 "dgn_comp.l"
return(RNDLEVEL);
	YY_BREAK
case 27:
# line 126 "dgn_comp.l"
return(CHLEVEL);
	YY_BREAK
case 28:
# line 127 "dgn_comp.l"
return(RNDCHLEVEL);
	YY_BREAK
case 29:
# line 128 "dgn_comp.l"
{ yylval.i=atoi(yytext); return(INTEGER); }
	YY_BREAK
case 30:
# line 129 "dgn_comp.l"
{ yytext[yyleng-1] = 0; /* Discard the trailing \" */
		  yylval.str = (char *) alloc(strlen(yytext+1)+1);
		  strcpy(yylval.str, yytext+1); /* Discard the first \" */
		  return(STRING); }
	YY_BREAK
case 31:
# line 133 "dgn_comp.l"
{ line_number++; }
	YY_BREAK
case 32:
# line 134 "dgn_comp.l"
{ line_number++; }
	YY_BREAK
case 33:
# line 135 "dgn_comp.l"
;	/* skip trailing tabs & spaces */
	YY_BREAK
case 34:
# line 136 "dgn_comp.l"
{ return yytext[0]; }
	YY_BREAK
case 35:
# line 137 "dgn_comp.l"
ECHO;
	YY_BREAK
case YY_STATE_EOF(INITIAL):
    yyterminate();

	    case YY_END_OF_BUFFER:
		{
		/* amount of text matched not including the EOB char */
		int yy_amount_of_matched_text = yy_cp - yytext - 1;

		/* undo the effects of YY_DO_BEFORE_ACTION */
		*yy_cp = yy_hold_char;

		/* note that here we test for yy_c_buf_p "<=" to the position
		 * of the first EOB in the buffer, since yy_c_buf_p will
		 * already have been incremented past the NUL character
		 * (since all states make transitions on EOB to the end-
		 * of-buffer state).  Contrast this with the test in yyinput().
		 */
		if ( yy_c_buf_p <= &yy_current_buffer->yy_ch_buf[yy_n_chars] )
		    /* this was really a NUL */
		    {
		    yy_state_type yy_next_state;

		    yy_c_buf_p = yytext + yy_amount_of_matched_text;

		    yy_current_state = yy_get_previous_state();

		    /* okay, we're now positioned to make the
		     * NUL transition.  We couldn't have
		     * yy_get_previous_state() go ahead and do it
		     * for us because it doesn't know how to deal
		     * with the possibility of jamming (and we
		     * don't want to build jamming into it because
		     * then it will run more slowly)
		     */

		    yy_next_state = yy_try_NUL_trans( yy_current_state );

		    yy_bp = yytext + YY_MORE_ADJ;

		    if ( yy_next_state )
			{
			/* consume the NUL */
			yy_cp = ++yy_c_buf_p;
			yy_current_state = yy_next_state;
			goto yy_match;
			}

		    else
			{
			    yy_cp = yy_last_accepting_cpos;
			    yy_current_state = yy_last_accepting_state;
			goto yy_find_action;
			}
		    }

		else switch ( yy_get_next_buffer() )
		    {
		    case EOB_ACT_END_OF_FILE:
			{
			yy_did_buffer_switch_on_eof = 0;

			if ( yywrap() )
			    {
			    /* note: because we've taken care in
			     * yy_get_next_buffer() to have set up yytext,
			     * we can now set up yy_c_buf_p so that if some
			     * total hoser (like flex itself) wants
			     * to call the scanner after we return the
			     * YY_NULL, it'll still work - another YY_NULL
			     * will get returned.
			     */
			    yy_c_buf_p = yytext + YY_MORE_ADJ;

			    yy_act = YY_STATE_EOF((yy_start - 1) / 2);
			    goto do_action;
			    }

			else
			    {
			    if ( ! yy_did_buffer_switch_on_eof )
				YY_NEW_FILE;
			    }
			}
			break;

		    case EOB_ACT_CONTINUE_SCAN:
			yy_c_buf_p = yytext + yy_amount_of_matched_text;

			yy_current_state = yy_get_previous_state();

			yy_cp = yy_c_buf_p;
			yy_bp = yytext + YY_MORE_ADJ;
			goto yy_match;

		    case EOB_ACT_LAST_MATCH:
			yy_c_buf_p =
			    &yy_current_buffer->yy_ch_buf[yy_n_chars];

			yy_current_state = yy_get_previous_state();

			yy_cp = yy_c_buf_p;
			yy_bp = yytext + YY_MORE_ADJ;
			goto yy_find_action;
		    }
		break;
		}

	    default:
#ifdef FLEX_DEBUG
		printf( "action # %d\n", yy_act );
#endif
		YY_FATAL_ERROR(
			"fatal flex scanner internal error--no action found" );
	    }
	}
    }


/* yy_get_next_buffer - try to read in a new buffer
 *
 * synopsis
 *     int yy_get_next_buffer();
 *     
 * returns a code representing an action
 *     EOB_ACT_LAST_MATCH - 
 *     EOB_ACT_CONTINUE_SCAN - continue scanning from current position
 *     EOB_ACT_END_OF_FILE - end of file
 */

static int yy_get_next_buffer()

    {
    register YY_CHAR *dest = yy_current_buffer->yy_ch_buf;
    register YY_CHAR *source = yytext - 1; /* copy prev. char, too */
    register int number_to_move, i;
    int ret_val;

    if ( yy_c_buf_p > &yy_current_buffer->yy_ch_buf[yy_n_chars + 1] )
	YY_FATAL_ERROR(
		"fatal flex scanner internal error--end of buffer missed" );

    /* try to read more data */

    /* first move last chars to start of buffer */
    number_to_move = yy_c_buf_p - yytext;

    for ( i = 0; i < number_to_move; ++i )
	*(dest++) = *(source++);

    if ( yy_current_buffer->yy_eof_status != EOF_NOT_SEEN )
	/* don't do the read, it's not guaranteed to return an EOF,
	 * just force an EOF
	 */
	yy_n_chars = 0;

    else
	{
	int num_to_read = yy_current_buffer->yy_buf_size - number_to_move - 1;

	if ( num_to_read > YY_READ_BUF_SIZE )
	    num_to_read = YY_READ_BUF_SIZE;

	else if ( num_to_read <= 0 )
	    YY_FATAL_ERROR( "fatal error - scanner input buffer overflow" );

	/* read in more data */
	YY_INPUT( (&yy_current_buffer->yy_ch_buf[number_to_move]),
		  yy_n_chars, num_to_read );
	}

    if ( yy_n_chars == 0 )
	{
	if ( number_to_move == 1 )
	    {
	    ret_val = EOB_ACT_END_OF_FILE;
	    yy_current_buffer->yy_eof_status = EOF_DONE;
	    }

	else
	    {
	    ret_val = EOB_ACT_LAST_MATCH;
	    yy_current_buffer->yy_eof_status = EOF_PENDING;
	    }
	}

    else
	ret_val = EOB_ACT_CONTINUE_SCAN;

    yy_n_chars += number_to_move;
    yy_current_buffer->yy_ch_buf[yy_n_chars] = YY_END_OF_BUFFER_CHAR;
    yy_current_buffer->yy_ch_buf[yy_n_chars + 1] = YY_END_OF_BUFFER_CHAR;

    /* yytext begins at the second character in yy_ch_buf; the first
     * character is the one which preceded it before reading in the latest
     * buffer; it needs to be kept around in case it's a newline, so
     * yy_get_previous_state() will have with '^' rules active
     */

    yytext = &yy_current_buffer->yy_ch_buf[1];

    return ( ret_val );
    }


/* yy_get_previous_state - get the state just before the EOB char was reached
 *
 * synopsis
 *     yy_state_type yy_get_previous_state();
 */

static yy_state_type yy_get_previous_state()

    {
    register yy_state_type yy_current_state;
    register YY_CHAR *yy_cp;

    register YY_CHAR *yy_bp = yytext;

    yy_current_state = yy_start;
    if ( yy_bp[-1] == '\n' )
	++yy_current_state;

    for ( yy_cp = yytext + YY_MORE_ADJ; yy_cp < yy_c_buf_p; ++yy_cp )
	{
	register YY_CHAR yy_c = (*yy_cp ? yy_ec[*yy_cp] : 1);
	if ( yy_accept[yy_current_state] )
	    {
	    yy_last_accepting_state = yy_current_state;
	    yy_last_accepting_cpos = yy_cp;
	    }
	while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
	    {
	    yy_current_state = yy_def[yy_current_state];
	    if ( yy_current_state >= 194 )
		yy_c = yy_meta[yy_c];
	    }
	yy_current_state = yy_nxt[yy_base[yy_current_state] + yy_c];
	}

    return ( yy_current_state );
    }


/* yy_try_NUL_trans - try to make a transition on the NUL character
 *
 * synopsis
 *     next_state = yy_try_NUL_trans( current_state );
 */

#ifdef YY_USE_PROTOS
static yy_state_type yy_try_NUL_trans( register yy_state_type yy_current_state )
#else
static yy_state_type yy_try_NUL_trans( yy_current_state )
register yy_state_type yy_current_state;
#endif

    {
    register int yy_is_jam;
    register YY_CHAR *yy_cp = yy_c_buf_p;

    register YY_CHAR yy_c = 1;
    if ( yy_accept[yy_current_state] )
	{
	yy_last_accepting_state = yy_current_state;
	yy_last_accepting_cpos = yy_cp;
	}
    while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
	{
	yy_current_state = yy_def[yy_current_state];
	if ( yy_current_state >= 194 )
	    yy_c = yy_meta[yy_c];
	}
    yy_current_state = yy_nxt[yy_base[yy_current_state] + yy_c];
    yy_is_jam = (yy_current_state == 193);

    return ( yy_is_jam ? 0 : yy_current_state );
    }


#ifdef YY_USE_PROTOS
static void yyunput( YY_CHAR c, register YY_CHAR *yy_bp )
#else
static void yyunput( c, yy_bp )
YY_CHAR c;
register YY_CHAR *yy_bp;
#endif

    {
    register YY_CHAR *yy_cp = yy_c_buf_p;

    /* undo effects of setting up yytext */
    *yy_cp = yy_hold_char;

    if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
	{ /* need to shift things up to make room */
	register int number_to_move = yy_n_chars + 2; /* +2 for EOB chars */
	register YY_CHAR *dest =
	    &yy_current_buffer->yy_ch_buf[yy_current_buffer->yy_buf_size + 2];
	register YY_CHAR *source =
	    &yy_current_buffer->yy_ch_buf[number_to_move];

	while ( source > yy_current_buffer->yy_ch_buf )
	    *--dest = *--source;

	yy_cp += dest - source;
	yy_bp += dest - source;
	yy_n_chars = yy_current_buffer->yy_buf_size;

	if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
	    YY_FATAL_ERROR( "flex scanner push-back overflow" );
	}

    if ( yy_cp > yy_bp && yy_cp[-1] == '\n' )
	yy_cp[-2] = '\n';

    *--yy_cp = c;

    /* note: the formal parameter *must* be called "yy_bp" for this
     *       macro to now work correctly
     */
    YY_DO_BEFORE_ACTION; /* set up yytext again */
    }


#ifdef __cplusplus
static int yyinput()
#else
static int input()
#endif

    {
    int c;
    YY_CHAR *yy_cp = yy_c_buf_p;

    *yy_cp = yy_hold_char;

    if ( *yy_c_buf_p == YY_END_OF_BUFFER_CHAR )
	{
	/* yy_c_buf_p now points to the character we want to return.
	 * If this occurs *before* the EOB characters, then it's a
	 * valid NUL; if not, then we've hit the end of the buffer.
	 */
	if ( yy_c_buf_p < &yy_current_buffer->yy_ch_buf[yy_n_chars] )
	    /* this was really a NUL */
	    *yy_c_buf_p = '\0';

	else
	    { /* need more input */
	    yytext = yy_c_buf_p;
	    ++yy_c_buf_p;

	    switch ( yy_get_next_buffer() )
		{
		case EOB_ACT_END_OF_FILE:
		    {
		    if ( yywrap() )
			{
			yy_c_buf_p = yytext + YY_MORE_ADJ;
			return ( EOF );
			}

		    YY_NEW_FILE;

#ifdef __cplusplus
		    return ( yyinput() );
#else
		    return ( input() );
#endif
		    }
		    break;

		case EOB_ACT_CONTINUE_SCAN:
		    yy_c_buf_p = yytext + YY_MORE_ADJ;
		    break;

		case EOB_ACT_LAST_MATCH:
#ifdef __cplusplus
		    YY_FATAL_ERROR( "unexpected last match in yyinput()" );
#else
		    YY_FATAL_ERROR( "unexpected last match in input()" );
#endif
		}
	    }
	}

    c = *yy_c_buf_p;
    yy_hold_char = *++yy_c_buf_p;

    return ( c );
    }


#ifdef YY_USE_PROTOS
void yyrestart( FILE *input_file )
#else
void yyrestart( input_file )
FILE *input_file;
#endif

    {
    yy_init_buffer( yy_current_buffer, input_file );
    yy_load_buffer_state();
    }


#ifdef YY_USE_PROTOS
void yy_switch_to_buffer( YY_BUFFER_STATE new_buffer )
#else
void yy_switch_to_buffer( new_buffer )
YY_BUFFER_STATE new_buffer;
#endif

    {
    if ( yy_current_buffer == new_buffer )
	return;

    if ( yy_current_buffer )
	{
	/* flush out information for old buffer */
	*yy_c_buf_p = yy_hold_char;
	yy_current_buffer->yy_buf_pos = yy_c_buf_p;
	yy_current_buffer->yy_n_chars = yy_n_chars;
	}

    yy_current_buffer = new_buffer;
    yy_load_buffer_state();

    /* we don't actually know whether we did this switch during
     * EOF (yywrap()) processing, but the only time this flag
     * is looked at is after yywrap() is called, so it's safe
     * to go ahead and always set it.
     */
    yy_did_buffer_switch_on_eof = 1;
    }


#ifdef YY_USE_PROTOS
void yy_load_buffer_state( void )
#else
void yy_load_buffer_state()
#endif

    {
    yy_n_chars = yy_current_buffer->yy_n_chars;
    yytext = yy_c_buf_p = yy_current_buffer->yy_buf_pos;
    yyin = yy_current_buffer->yy_input_file;
    yy_hold_char = *yy_c_buf_p;
    }


#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_create_buffer( FILE *file, int size )
#else
YY_BUFFER_STATE yy_create_buffer( file, size )
FILE *file;
int size;
#endif

    {
    YY_BUFFER_STATE b;

    b = (YY_BUFFER_STATE) malloc( sizeof( struct yy_buffer_state ) );

    if ( ! b )
	YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

    b->yy_buf_size = size;

    /* yy_ch_buf has to be 2 characters longer than the size given because
     * we need to put in 2 end-of-buffer characters.
     */
    b->yy_ch_buf = (YY_CHAR *) malloc( (unsigned) (b->yy_buf_size + 2) );

    if ( ! b->yy_ch_buf )
	YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

    yy_init_buffer( b, file );

    return ( b );
    }


#ifdef YY_USE_PROTOS
void yy_delete_buffer( YY_BUFFER_STATE b )
#else
void yy_delete_buffer( b )
YY_BUFFER_STATE b;
#endif

    {
    if ( b == yy_current_buffer )
	yy_current_buffer = (YY_BUFFER_STATE) 0;

    free( (char *) b->yy_ch_buf );
    free( (char *) b );
    }


#ifdef YY_USE_PROTOS
void yy_init_buffer( YY_BUFFER_STATE b, FILE *file )
#else
void yy_init_buffer( b, file )
YY_BUFFER_STATE b;
FILE *file;
#endif

    {
    b->yy_input_file = file;

    /* we put in the '\n' and start reading from [1] so that an
     * initial match-at-newline will be true.
     */

    b->yy_ch_buf[0] = '\n';
    b->yy_n_chars = 1;

    /* we always need two end-of-buffer characters.  The first causes
     * a transition to the end-of-buffer state.  The second causes
     * a jam in that state.
     */
    b->yy_ch_buf[1] = YY_END_OF_BUFFER_CHAR;
    b->yy_ch_buf[2] = YY_END_OF_BUFFER_CHAR;

    b->yy_buf_pos = &b->yy_ch_buf[1];

    b->yy_eof_status = EOF_NOT_SEEN;
    }
# line 137 "dgn_comp.l"


/* routine to switch to another input file; needed for flex */
void init_yyin( input_f )
FILE *input_f;
{
#ifdef FLEX_SCANNER
	if (yyin)
	    yyrestart(input_f);
	else
#endif
	    yyin = input_f;
}
/* analogous routine (for completeness) */
void init_yyout( output_f )
FILE *output_f;
{
	yyout = output_f;
}

