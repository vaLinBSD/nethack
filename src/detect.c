/*	SCCS Id: @(#)detect.c	3.1	92/12/16	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

/*
 * Detection routines, including crystal ball, magic mapping, and search
 * command.
 */

#include "hack.h"
#include "artifact.h"

extern boolean known;	/* from read.c */

static boolean FDECL(check_map_spot, (int,int,CHAR_P));
static boolean FDECL(clear_stale_map, (CHAR_P));
static void FDECL(sense_trap, (struct trap *,XCHAR_P,XCHAR_P,int));
static void FDECL(show_map_spot, (int,int));
STATIC_PTR void FDECL(findone,(int,int,genericptr_t));
STATIC_PTR void FDECL(openone,(int,int,genericptr_t));

/* Recursively search obj for an object in class oclass and return 1st found */
struct obj *
o_in(obj, oclass)
struct obj* obj;
char oclass;
{
    register struct obj* otmp;
    struct obj *temp;

    if (obj->oclass == oclass) return obj;

    if (Is_container(obj)) {
	for (otmp = obj->cobj; otmp; otmp = otmp->nobj)
	    if (otmp->oclass == oclass) return otmp;
	    else if (Is_container(otmp) && (temp = o_in(otmp, oclass)))
		return temp;
    }
    return (struct obj *) 0;
}

/* Check whether the location has an outdated object displayed on it. */
static boolean
check_map_spot(x, y, oclass)
int x, y;
register char oclass;
{
	register struct rm *lev;
	register struct obj *otmp;
	register struct monst *mtmp;

	lev = &levl[x][y];
	if (glyph_is_object(lev->glyph)) {
	    /* there's some object shown here */
	    if (oclass == ALL_CLASSES) {
		return !(level.objects[x][y] ||     /* stale if nothing here */
			    ((mtmp = m_at(x,y)) != 0 &&
				(mtmp->mgold || mtmp->minvent)));
	    } else if (objects[glyph_to_obj(lev->glyph)].oc_class == oclass) {
		/* the object shown here is of interest */
		for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
		    if (o_in(otmp, oclass)) return FALSE;
		/* didn't find it; perhaps a monster is carrying it */
		if ((mtmp = m_at(x,y)) != 0) {
		    if (oclass == GOLD_CLASS && mtmp->mgold)
			return FALSE;
		    else for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
			    if (o_in(otmp, oclass)) return FALSE;
		}
		/* detection indicates removal of this object from the map */
		return TRUE;
	    }
	}
	return FALSE;
}

/*
   When doing detection, remove stale data from the map display (corpses
   rotted away, objects carried away by monsters, etc) so that it won't
   reappear after the detection has completed.  Return true if noticeable
   change occurs.
 */
static boolean
clear_stale_map(oclass)
register char oclass;
{
	register int zx, zy;
	register boolean change_made = FALSE;

	for (zx = 1; zx < COLNO; zx++)
	    for (zy = 0; zy < ROWNO; zy++)
		if (check_map_spot(zx, zy, oclass)) {
		    unmap_object(zx, zy);
		    change_made = TRUE;
		}

	return change_made;
}

/* look for gold, on the floor or in monsters' possession */
int
gold_detect(sobj)
register struct obj *sobj;
{
    register struct obj *obj;
    register struct monst *mtmp;
    int uw = u.uinwater;
    struct obj *temp;
    boolean stale;

    known = stale = clear_stale_map(GOLD_CLASS);

    /* look for gold carried by monsters (might be in a container) */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
	if (mtmp->mgold) {
	    known = TRUE;
	    goto outgoldmap;	/* skip further searching */
	} else for (obj = mtmp->minvent; obj; obj = obj->nobj)
	    if (o_in(obj, GOLD_CLASS)) {
		known = TRUE;
		goto outgoldmap;	/* skip further searching */
	    }

    /* look for gold objects */
    for (obj = fobj; obj; obj = obj->nobj)
	if (o_in(obj, GOLD_CLASS)) {
	    known = TRUE;
	    if (obj->ox != u.ux || obj->oy != u.uy) goto outgoldmap;
	}

    if (!known) {
	/* no gold found */
	if (sobj) strange_feeling(sobj, "You feel materially poor.");
	return(1);
    }
    /* only under me - no separate display required */
    if (stale) docrt();
    You("notice some gold between your %s.", makeplural(body_part(FOOT)));
    return(0);

outgoldmap:
    cls();

    u.uinwater = 0;
    /* Discover gold locations. */
    for (obj = fobj; obj; obj = obj->nobj)
	if ((temp = o_in(obj, GOLD_CLASS))) {
	    if (temp != obj) {
		temp->ox = obj->ox;
		temp->oy = obj->oy;
	    }
	    map_object(temp,1);
	}
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
	if (mtmp->mgold) {
	    struct obj gold;

	    gold.otyp = GOLD_PIECE;
	    gold.ox = mtmp->mx;
	    gold.oy = mtmp->my;
	    map_object(&gold,1);
	} else for (obj = mtmp->minvent; obj; obj = obj->nobj)
	    if ((temp = o_in(obj, GOLD_CLASS))) {
		temp->ox = mtmp->mx;
		temp->oy = mtmp->my;
		map_object(temp,1);
		break;
	    }

    newsym(u.ux,u.uy);
    You("feel very greedy, and sense gold!");
    exercise(A_WIS, TRUE);
    display_nhwindow(WIN_MAP, TRUE);
    docrt();
    u.uinwater = uw;
    if (Underwater) under_water(2);
    return(0);
}

/* returns 1 if nothing was detected		*/
/* returns 0 if something was detected		*/
int
food_detect(sobj)
register struct obj	*sobj;
{
    register struct obj *obj;
    register struct monst *mtmp;
    register int ct = 0, ctu = 0;
    boolean confused = (Confusion || (sobj && sobj->cursed)), stale;
    char oclass = confused ? POTION_CLASS : FOOD_CLASS;
    const char *what = confused ? "something" : "food";
    int uw = u.uinwater;

    stale = clear_stale_map(oclass);

    for (obj = fobj; obj; obj = obj->nobj)
	if (o_in(obj, oclass)) {
	    if (obj->ox == u.ux && obj->oy == u.uy) ctu++;
	    else ct++;
	}
    for (mtmp = fmon; mtmp && !ct; mtmp = mtmp->nmon)
	for (obj = mtmp->minvent; obj; obj = obj->nobj)
	    if (o_in(obj, oclass)) {
		ct++;
		break;
	    }

    if (!ct && !ctu) {
	known = stale && !confused;
	if (stale) {
	    docrt();
	    You("sense lack of %s nearby.", what);
	} else if (sobj)
	    strange_feeling(sobj, "Your nose twitches.");
	return !stale;
    } else if (!ct) {
	known = TRUE;
	You("%s %s nearby.", sobj ? "smell" : "sense", what);
    } else {
	struct obj *temp;
	known = TRUE;
	cls();
	u.uinwater = 0;
	for (obj = fobj; obj; obj = obj->nobj)
	    if ((temp = o_in(obj, oclass)) != 0) {
		if (temp != obj) {
		    temp->ox = obj->ox;
		    temp->oy = obj->oy;
		}
		map_object(temp,1);
	    }
	for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
	    for (obj = mtmp->minvent; obj; obj = obj->nobj)
		if ((temp = o_in(obj, oclass)) != 0) {
		    temp->ox = mtmp->mx;
		    temp->oy = mtmp->my;
		    map_object(temp,1);
		    break;	/* skip rest of this monster's inventory */
		}
	newsym(u.ux,u.uy);
	if (sobj) Your("nose tingles and you smell %s.", what);
	else You("sense %s.", what);
	display_nhwindow(WIN_MAP, TRUE);
	exercise(A_WIS, TRUE);
	docrt();
	u.uinwater = uw;
	if (Underwater) under_water(2);
    }
    return(0);
}

/*
 * Used for scrolls, potions, and crystal balls.  Returns:
 *
 *	1 - nothing was detected
 *	0 - something was detected
 */
int
object_detect(detector, class)
struct obj	*detector;	/* object doing the detecting */
int		class;		/* an object class, 0 for all */
{
    register int x, y;
    int is_cursed = (detector && detector->cursed);
    int ct = 0, ctu = 0;
    register struct obj *obj, *otmp = (struct obj *)0;
    register struct monst *mtmp;
    int uw = u.uinwater;
    const char *stuff;

    if (class < 0 || class >= MAXOCLASSES) {
	impossible("object_detect:  illegal class %d", class);
	class = 0;
    }

    if (Hallucination || (Confusion && class == SCROLL_CLASS))
	stuff = "something";
    else
	stuff = class ? oclass_names[class] : "objects";

    for (obj = fobj; obj && !ct; obj = obj->nobj)
	if (!class || o_in(obj, class)) {
	    if (obj->ox == u.ux && obj->oy == u.uy) ctu++;
	    else ct++;
	}

    if (!ct)
	for (mtmp = fmon; mtmp && !ct; mtmp = mtmp->nmon) {
	    if (!class && mtmp->minvent) {
		ct++;
		break;
	    }

	    for (obj = mtmp->minvent; obj; obj = obj->nobj)
		if (o_in(obj, class)) {
		    ct++;
		    break;
		}

	    if ((is_cursed && mtmp->m_ap_type == M_AP_OBJECT &&
		(!class || class == objects[mtmp->mappearance].oc_class)) ||
		(mtmp->mgold && (!class || class == GOLD_CLASS))) {
		ct++;
		break;
	    }
	}

    if (!clear_stale_map(!class ? ALL_CLASSES : class) && !ct) {
	if (!ctu) {
	    if (detector)
		strange_feeling(detector, "You feel a lack of something.");
	    return 1;
	}

	You("sense %s nearby.", stuff);
	return 0;
    }

    cls();

    u.uinwater = 0;
    /*
     * If we are mapping all objects, map only the top object of a pile or
     * the first object in a monster's inventory.  Otherwise, go looking
     * for a matching object class and display the first one encountered
     * at each location.
     */
    for (x = 1; x < COLNO; x++)
	for (y = 0; y < ROWNO; y++)
	    for (obj = level.objects[x][y]; obj; obj = obj->nexthere)
		if (!class || (otmp = o_in(obj, class))) {
		    if (class) {
			if (otmp != obj) {
			    otmp->ox = obj->ox;
			    otmp->oy = obj->oy;
			}
			map_object(otmp, 1);
		    } else
			map_object(obj, 1);
		    break;
		}

    /* Objects in the monster's inventory override floor objects. */
    for (mtmp = fmon ; mtmp ; mtmp = mtmp->nmon) {
	for (obj = mtmp->minvent; obj; obj = obj->nobj)
	    if (!class || (otmp = o_in(obj, class))) {
		if (!class) otmp = obj;
		otmp->ox = mtmp->mx;		/* at monster location */
		otmp->oy = mtmp->my;
		map_object(otmp, 1);
		break;
	    }


	/* Allow a mimic to override the detected objects it is carrying. */
	if (is_cursed && mtmp->m_ap_type == M_AP_OBJECT &&
		(!class || class == objects[mtmp->mappearance].oc_class)) {
	    struct obj temp;

	    temp.otyp = mtmp->mappearance;	/* needed for obj_to_glyph() */
	    temp.ox = mtmp->mx;
	    temp.oy = mtmp->my;
	    temp.corpsenm = PM_TENGU;		/* if mimicing a corpse */
	    map_object(&temp, 1);
	} else if (mtmp->mgold && (!class || class == GOLD_CLASS)) {
	    struct obj gold;

	    gold.otyp = GOLD_PIECE;
	    gold.ox = mtmp->mx;
	    gold.oy = mtmp->my;
	    map_object(&gold, 1);
	}
    }

    newsym(u.ux,u.uy);
    You("detect the %s of %s.", ct ? "presence" : "absence", stuff);
    display_nhwindow(WIN_MAP, TRUE);
    /*
     * What are we going to do when the hero does an object detect while blind
     * and the detected object covers a known pool?
     */
    docrt();	/* this will correctly reset vision */

    u.uinwater = uw;
    if (Underwater) under_water(2);
    return 0;
}

/*
 * Used by: crystal balls, potions, fountains
 *
 * Returns 1 if nothing was detected.
 * Returns 0 if something was detected.
 */
int
monster_detect(otmp, mclass)
register struct obj *otmp;	/* detecting object (if any) */
int		    mclass;	/* monster class, 0 for all */
{
    register struct monst *mtmp;

    if (!fmon) {
	if (otmp)
	    strange_feeling(otmp, Hallucination ?
			    "You get the heebie jeebies." :
			    "You feel threatened.");
	return 1;
    } else {
	int woken = FALSE;

	cls();
	for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
	    if (!mclass || mtmp->data->mlet == mclass)
	    if (mtmp->mx > 0)
		show_glyph(mtmp->mx,mtmp->my,mon_to_glyph(mtmp));
	    if (otmp && otmp->cursed &&
		(mtmp->msleep || !mtmp->mcanmove)) {
		mtmp->msleep = mtmp->mfrozen = 0;
		mtmp->mcanmove = 1;
		woken = TRUE;
	    }
	}
	display_self();
	You("sense the presence of monsters.");
	if (woken)
	    pline("Monsters sense the presence of you.");
	display_nhwindow(WIN_MAP, TRUE);
	docrt();
	if (Underwater) under_water(2);
    }
    return 0;
}

static void
sense_trap(trap, x, y, src_cursed)
struct trap *trap;
xchar	    x, y;
int	    src_cursed;
{
    if (Hallucination || src_cursed) {
	struct obj obj;			/* fake object */
	if (trap) {
	    obj.ox = trap->tx;
	    obj.oy = trap->ty;
	} else {
	    obj.ox = x;
	    obj.oy = y;
	}
	obj.otyp = (src_cursed) ? GOLD_PIECE : random_object();
	obj.corpsenm = random_monster();	/* if otyp == CORPSE */
	map_object(&obj,1);
    } else if (trap) {
	map_trap(trap,1);
	trap->tseen = 1;
    } else {
	struct trap temp_trap;		/* fake trap */
	temp_trap.tx = x;
	temp_trap.ty = y;
	temp_trap.ttyp = BEAR_TRAP;	/* some kind of trap */
	map_trap(&temp_trap,1);
    }

}

/* the detections are pulled out so they can	*/
/* also be used in the crystal ball routine	*/
/* returns 1 if nothing was detected		*/
/* returns 0 if something was detected		*/
int
trap_detect(sobj)
register struct obj *sobj;
/* sobj is null if crystal ball, *scroll if gold detection scroll */
{
    register struct trap *ttmp;
    register struct obj *obj;
    register int door;
    int uw = u.uinwater;
    boolean found = FALSE;
    coord cc;

    for (ttmp = ftrap; ttmp; ttmp = ttmp->ntrap) {
	if (ttmp->tx != u.ux || ttmp->ty != u.uy)
	    goto outtrapmap;
	else found = TRUE;
    }
    for (obj = fobj; obj; obj = obj->nobj) {
	if ((obj->otyp==LARGE_BOX || obj->otyp==CHEST) && obj->otrapped)
	    if (obj->ox != u.ux || obj->oy != u.uy)
		goto outtrapmap;
	    else found = TRUE;
    }
    for (door = 0; door <= doorindex; door++) {
	cc = doors[door];
	if (levl[cc.x][cc.y].doormask & D_TRAPPED)
	    if (cc.x != u.ux || cc.x != u.uy)
		goto outtrapmap;
	    else found = TRUE;
    }
    if (!found) {
	char buf[42];
	Sprintf(buf, "Your %s stop itching.", makeplural(body_part(TOE)));
	strange_feeling(sobj,buf);
	return(1);
    }
    /* traps exist, but only under me - no separate display required */
    Your("%s itch.", makeplural(body_part(TOE)));
    return(0);
outtrapmap:
    cls();

    u.uinwater = 0;
    for (ttmp = ftrap; ttmp; ttmp = ttmp->ntrap)
	sense_trap(ttmp, 0, 0, sobj && sobj->cursed);

    for (obj = fobj; obj; obj = obj->nobj)
	if ((obj->otyp==LARGE_BOX || obj->otyp==CHEST) && obj->otrapped)
	sense_trap((struct trap *)0, obj->ox, obj->oy, sobj && sobj->cursed);

    for (door = 0; door <= doorindex; door++) {
	cc = doors[door];
	if (levl[cc.x][cc.y].doormask & D_TRAPPED)
	sense_trap((struct trap *)0, cc.x, cc.y, sobj && sobj->cursed);
    }

    newsym(u.ux,u.uy);
    You("feel %s.", sobj && sobj->cursed ? "very greedy" : "entrapped");
    display_nhwindow(WIN_MAP, TRUE);
    docrt();
    u.uinwater = uw;
    if (Underwater) under_water(2);
    return(0);
}

const char *
level_distance(where)
d_level *where;
{
    register schar ll = depth(&u.uz) - depth(where);
    register boolean indun = (u.uz.dnum == where->dnum);

    if (ll < 0) {
    	if (ll < (-8 - rn2(3)))
	    if (!indun)	return "far away";
	    else	return "far below";
	else if (ll < -1)
	    if (!indun)	return "away below you";
	    else	return "below you";
	else
	    if (!indun)	return "in the distance";
	    else	return "just below";
    } else if (ll > 0) {
	if (ll > (8 + rn2(3)))
	    if (!indun)	return "far away";
	    else	return "far above";
	else if (ll > 1)
	    if (!indun)	return "away above you";
	    else	return "above you";
	else
	    if (!indun)	return "in the distance";
	    else	return "just above";
    } else
	    if (!indun)	return "in the distance";
	    else	return "near you";
}

static struct {
    const char *what;
    d_level *where;
} level_detects[] = {
  { "Delphi", &oracle_level },
  { "Medusa's lair", &medusa_level },
  { "a castle", &stronghold_level },
  { "the Wizard of Yendor's tower", &wiz1_level },
};

void
use_crystal_ball(obj)
struct obj *obj;
{
    char ch;
    int oops;
    const char *bname = xname(obj);

    if (Blind) {
	pline("Too bad you can't see %s", the(bname));
	return;
    }
    oops = (rnd(20) > ACURR(A_INT) || obj->cursed);
    if (oops && (obj->spe > 0)) {
	switch(rnd(5)) {
	case 1 : pline("%s is too much to comprehend!", The(bname));
	    break;
	case 2 : pline("%s confuses you!", The(bname));
	    make_confused(HConfusion + rnd(100),FALSE);
	    break;
	case 3 : pline("%s damages your vision!", The(bname));
	    make_blinded(Blinded + rnd(100),FALSE);
	    break;
	case 4 : pline("%s zaps your mind!", The(bname));
	    make_hallucinated(HHallucination + rnd(100),FALSE,0L);
	    break;
	case 5 : pline("%s explodes!", The(bname));
	    useup(obj);
	    losehp(rnd(30), "exploding crystal ball", KILLED_BY_AN);
	    break;
	}
	obj->spe--;
	return;
    }

    if (Hallucination) {
	if (!obj->spe) {
	    pline("All you see is funky %s haze.", hcolor());
	} else {
	    switch(rnd(6)) {
	    case 1 : You("grok some groovy globs of incandescent lava.");
		break;
	    case 2 : pline("Whoa!  Psychedelic colors, %s!",
			   poly_gender() == 1 ? "babe" : "dude");
		break;
	    case 3 : pline("The crystal pulses with sinister %s light!", hcolor());
		break;
	    case 4 : You("see goldfish swimming above fluorescent rocks.");
		break;
	    case 5 : You("see tiny snowflakes spinning around a miniature farmhouse.");
		break;
	    default: pline("Oh wow... like a kaleidoscope!");
		break;
	    }
	    obj->spe--;
	}
	return;
    }

    /* read a single character */
    if (flags.verbose) You("may look for an object or monster symbol.");
    ch = yn_function("What do you look for?", NULL, '\0');
    if (index(quitchars,ch)) {
	if (flags.verbose) pline("Never mind.");
	return;
    }
    You("peer into %s...", the(bname));
    nomul(-rnd(10));
    nomovemsg = "";
    if (obj->spe <= 0)
	pline("The vision is unclear.");
    else {
	int class;
	int ret = 0;

	makeknown(CRYSTAL_BALL);
	obj->spe--;

	if ((class = def_char_to_objclass(ch)) != MAXOCLASSES)
		ret = object_detect((struct obj *)0, class);
	else if ((class = def_char_to_monclass(ch)) != MAXMCLASSES)
		ret = monster_detect((struct obj *)0, class);
	else switch(ch) {
		case '^':
		    ret = trap_detect((struct obj *)0);
		    break;
		default:
		    {
		    int i = rn2(SIZE(level_detects));
		    You("see %s, %s.",
			level_detects[i].what,
			level_distance(level_detects[i].where));
		    }
		    ret = 0;
		    break;
	}

	if (ret) {
	    if (!rn2(100))  /* make them nervous */
		You("see the Wizard of Yendor gazing out at you.");
	    else pline("The vision is unclear.");
	}
    }
    return;
}

static void
show_map_spot(x, y)
register int x, y;
{
    register struct rm *lev;

    if (Confusion && rn2(7)) return;
    lev = &levl[x][y];

    if (IS_WALL(lev->typ) || lev->typ == SDOOR)
	lev->seen = 1;		/* We have seen all the walls. */

    /* Secret corridors are found, but not secret doors. */
    if (lev->typ == SCORR) {
	lev->typ = CORR;
	unblock_point(x,y);
    }

    /* if not a room and we don't remember an object or trap there, map it */
    if (lev->typ != ROOM  &&
		!(glyph_is_object(lev->glyph) || glyph_is_trap(lev->glyph))) {
	if (level.flags.hero_memory) {
	    map_background(x,y,0);
	    newsym(x,y);		/* show it, if not blocked */
	} else {
	    map_background(x,y,1);	/* display it */
	}
    }
}

void
do_mapping()
{
    register int zx, zy;
    int uw = u.uinwater;

    u.uinwater = 0;
    for (zx = 1; zx < COLNO; zx++)
	for (zy = 0; zy < ROWNO; zy++)
	    show_map_spot(zx, zy);
    exercise(A_WIS, TRUE);
    u.uinwater = uw;
    if (!level.flags.hero_memory || Underwater) {
	flush_screen(1);			/* flush temp screen */
	display_nhwindow(WIN_MAP, TRUE);	/* wait */
	docrt();
    }
}

void
do_vicinity_map()
{
    register int zx, zy;
    int lo_y = (u.uy-5 < 0 ? 0 : u.uy-5),
	hi_y = (u.uy+6 > ROWNO ? ROWNO : u.uy+6),
	lo_x = (u.ux-9 < 1 ? 1 : u.ux-9),	/* avoid column 0 */
	hi_x = (u.ux+10 > COLNO ? COLNO : u.ux+10);

    for (zx = lo_x; zx < hi_x; zx++)
	for (zy = lo_y; zy < hi_y; zy++)
	    show_map_spot(zx, zy);

    if (!level.flags.hero_memory || Underwater) {
	flush_screen(1);			/* flush temp screen */
	display_nhwindow(WIN_MAP, TRUE);	/* wait */
	docrt();
    }
}


STATIC_PTR void
findone(zx,zy,num)
int zx,zy;
genericptr_t num;
{
	register struct trap *ttmp;
	register struct monst *mtmp;

	if(levl[zx][zy].typ == SDOOR) {
		levl[zx][zy].typ = DOOR;
		newsym(zx, zy);
		(*(int*)num)++;
	} else if(levl[zx][zy].typ == SCORR) {
		levl[zx][zy].typ = CORR;
		newsym(zx, zy);
		(*(int*)num)++;
	} else if ((ttmp = t_at(zx, zy)) != 0) {
		if(!ttmp->tseen && ttmp->ttyp != STATUE_TRAP) {
			ttmp->tseen = 1;
			newsym(zx,zy);
			(*(int*)num)++;
		}
	} else if ((mtmp = m_at(zx, zy)) != 0) {
		if(mtmp->m_ap_type) {
			seemimic(mtmp);
			(*(int*)num)++;
		}
		if (mtmp->mundetected && is_hider(mtmp->data)) {
			mtmp->mundetected = 0;
			newsym(zx, zy);
			(*(int*)num)++;
		}
	}
}

STATIC_PTR void
openone(zx,zy,num)
int zx,zy;
genericptr_t num;
{
	register struct trap *ttmp;
	register struct obj *otmp;

	if(OBJ_AT(zx, zy)) {
		for(otmp = level.objects[zx][zy];
				otmp; otmp = otmp->nexthere) {
		    if(Is_box(otmp) && otmp->olocked) {
			otmp->olocked = 0;
			(*(int*)num)++;
		    }
		}
		/* let it fall to the next cases. could be on trap. */
	}
	if(levl[zx][zy].typ == SDOOR || (levl[zx][zy].typ == DOOR &&
		      (levl[zx][zy].doormask & (D_CLOSED|D_LOCKED)))) {
		if(levl[zx][zy].typ == SDOOR)
		    levl[zx][zy].typ = DOOR;
		if(levl[zx][zy].doormask & D_TRAPPED) {
		    if(distu(zx, zy) < 3) b_trapped("door");
		    else Norep("You %s an explosion!",
				cansee(zx, zy) ? "see" :
				   (flags.soundok ? "hear" :
						"feel the shock of"));
		    levl[zx][zy].doormask = D_NODOOR;
		} else
		    levl[zx][zy].doormask = D_ISOPEN;
		newsym(zx, zy);
		(*(int*)num)++;
	} else if(levl[zx][zy].typ == SCORR) {
		levl[zx][zy].typ = CORR;
		newsym(zx, zy);
		(*(int*)num)++;
	} else if ((ttmp = t_at(zx, zy)) != 0) {
		if (!ttmp->tseen && ttmp->ttyp != STATUE_TRAP) {
		    ttmp->tseen = 1;
		    newsym(zx,zy);
		    (*(int*)num)++;
		}
	} else if (find_drawbridge(&zx, &zy)) {
		/* make sure it isn't an open drawbridge */
		open_drawbridge(zx, zy);
		(*(int*)num)++;
	}
}

int
findit()	/* returns number of things found */
{
	int num = 0;

	if(u.uswallow) return(0);
	do_clear_area(u.ux, u.uy, BOLT_LIM, findone, (genericptr_t) &num);
	return(num);
}

int
openit()	/* returns number of things found and opened */
{
	int num = 0;

	if(u.uswallow) {
		if (is_animal(u.ustuck->data)) {
			if (Blind) pline("Its mouth opens!");
			else pline("%s opens its mouth!", Monnam(u.ustuck));
		}
		expels(u.ustuck, u.ustuck->data, TRUE);
		return(-1);
	}

	do_clear_area(u.ux, u.uy, BOLT_LIM, openone, (genericptr_t) &num);
	return(num);
}

int
dosearch0(aflag)
register int aflag;
{
#ifdef GCC_BUG
/* some versions of gcc seriously muck up nested loops. if you get strange
   crashes while searching in a version compiled with gcc, try putting
   #define GCC_BUG in *conf.h (or adding -DGCC_BUG to CFLAGS in the
   makefile).
 */
	volatile xchar x, y;
#else
	register xchar x, y;
#endif
	register struct trap *trap;
	register struct monst *mtmp;
	register struct obj *otmp;

	if(u.uswallow) {
		if (!aflag)
			pline("What are you looking for?  The exit?");
	} else {
	    int fund = (uwep && uwep->oartifact &&
		    spec_ability(uwep, SPFX_SEARCH)) ?
			((uwep->spe > 5) ? 5 : uwep->spe) : 0;
	    for(x = u.ux-1; x < u.ux+2; x++)
	      for(y = u.uy-1; y < u.uy+2; y++) {
		if(!isok(x,y)) continue;
		if(x != u.ux || y != u.uy) {
		    if (Blind && !aflag) feel_location(x,y);
		    if(levl[x][y].typ == SDOOR) {
			if(rnl(7-fund)) continue;
			levl[x][y].typ = DOOR;
			exercise(A_WIS, TRUE);
			nomul(0);
			if (Blind && !aflag)
			    feel_location(x,y);	/* make sure it shows up */
			else
			    newsym(x,y);
		    } else if(levl[x][y].typ == SCORR) {
			if(rnl(7-fund)) continue;
			levl[x][y].typ = CORR;
			unblock_point(x,y);	/* vision */
			exercise(A_WIS, TRUE);
			nomul(0);
			newsym(x,y);
		    } else {
		/* Be careful not to find anything in an SCORR or SDOOR */
			if(!aflag && (mtmp = m_at(x, y))) {
			    if(mtmp->m_ap_type) {
				seemimic(mtmp);
				exercise(A_WIS, TRUE);
				if (!canseemon(mtmp) && !sensemon(mtmp))
				    You("feel an invisible monster!");
				else
				    You("find %s.", a_monnam(mtmp));
				return(1);
			    }
			    if(mtmp->mundetected && is_hider(mtmp->data)) {
				mtmp->mundetected = 0;
				newsym(x,y);
				exercise(A_WIS, TRUE);
				if (!canseemon(mtmp) && !sensemon(mtmp))
				    You("feel an invisible monster!");
				else
				    You("find %s.", a_monnam(mtmp));
				return(1);
			    }
			}

			if ((trap = t_at(x,y)) && !trap->tseen && !rnl(8)) {
			    nomul(0);

			    if (trap->ttyp == STATUE_TRAP) {
				if ((otmp = sobj_at(STATUE, x, y)) != 0) {
				    mtmp = makemon(&mons[otmp->corpsenm], x, y);
				    if (mtmp)
					You("find %s posing as a statue.",
						  a_monnam(mtmp));
				    delobj(otmp);
				    exercise(A_WIS, TRUE);
				    if(Blind)
					feel_location(x,y);
				    else
					newsym(x, y);
				}
				deltrap(trap);
				return(1);
			    } else {
				You("find a%s.", traps[Hallucination ?
					    rn1(TRAPNUM-3, 2) : trap->ttyp ]);
				trap->tseen = 1;
				exercise(A_WIS, TRUE);
				if(Blind)
				    feel_location(x,y);
				else
				    newsym(x,y);
			    }
			}
		    }
		}
	    }
	}
	return(1);
}

int
dosearch()
{
	return(dosearch0(0));
}

/*detect.c*/
