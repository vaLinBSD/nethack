/*	SCCS Id: @(#)sit.c	3.1	92/10/24	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "artifact.h"

void
take_gold()
{
	if (u.ugold <= 0)  {
		You("feel a strange sensation.");
	} else {
		You("notice you have no gold!");
		u.ugold = 0;
		flags.botl = 1;
	}
}

int
dosit()
{
	char pbuf[BUFSZ];
	register struct trap *trap;
	register int typ = levl[u.ux][u.uy].typ;

	if(Levitation)  {
	    pline("There's nothing to sit on up here.");
	    return 0;
	} 

	Strcpy(pbuf, "You sit on the %s.");

	if(OBJ_AT(u.ux, u.uy)) { 
	    register struct obj *obj;

	    obj = level.objects[u.ux][u.uy];
	    You("sit on %s.", the(xname(obj)));
	    if(!Is_box(obj)) pline("It is not very comfortable...");

	} else if(trap = t_at(u.ux, u.uy)) {

	    if(u.utrap) {
	        if(u.utraptype == TT_BEARTRAP) {
		    You("can't sit down!");
		    u.utrap++;
	        } else if(u.utraptype == TT_PIT) {
		    You("sit in the bottom of the pit.");
		    if(trap->ttyp == SPIKED_PIT) pline("This hurts!");
		    u.utrap += rn2(5);
		} else if(u.utraptype == TT_WEB) {
		    pline(pbuf, "giant spider's web");
		    You("are further entangled!");
		    u.utrap += rn1(10, 5);
		} else if(u.utraptype == TT_LAVA) {
		    /* Must have fire resistance or they'd be dead already */
		    You("sit in the lava!");
		    u.utrap += rnd(4);
		    losehp(d(2,10), "sitting in lava", KILLED_BY);
		} else if(u.utraptype == TT_INFLOOR) {
		    You("can't maneuver to sit!");
		    u.utrap++;
		}
	    } else {
	        You("sit down.");
		dotrap(trap);
	    }
	} else if(Underwater || Is_waterlevel(&u.uz)) {
	    if (Is_waterlevel(&u.uz))
		pline("There are no seats floating in the neighborhood.");
	    else
		You("sit down in the muddy bottom.");
	} else if(is_pool(u.ux, u.uy)) {

	    You("sit in the water.");
	    if (!rn2(10) && uarm)
		(void) rust_dmg(uarm, "armor", 1, TRUE);
#ifdef POLYSELF
	    /* Note: without POLYSELF, this can't _happen_ without */
	    /* water walking boots.... */
	    if (!rn2(10) && uarmf && uarmf->otyp != WATER_WALKING_BOOTS)
		(void) rust_dmg(uarm, "armor", 1, TRUE);
#endif
#ifdef SINKS
	} else if(IS_SINK(typ)) {

	    pline(pbuf, defsyms[S_sink].explanation);
	    Your("%s gets wet.", humanoid(uasmon) ? "rump" : "underside");
#endif
	} else if(IS_ALTAR(typ)) {

	    pline(pbuf, defsyms[S_altar].explanation);
	    altar_wrath(u.ux, u.uy);

	} else if(typ == STAIRS) {

	    pline(pbuf, "stairs");

	} else if(typ == LADDER) {

	    pline(pbuf, "ladder");

	} else if (is_lava(u.ux, u.uy)) {

	    /* must be WWalking */
	    pline(pbuf, "lava");
	    pline("It burns you!");
	    losehp(d((Fire_resistance ? 2 : 10), 10),
		   "sitting on lava", KILLED_BY);

	} else if (is_ice(u.ux, u.uy)) {

	    pline(pbuf, defsyms[S_ice].explanation);
	    if (!Cold_resistance) pline("It's very cold...");

	} else if (typ == DRAWBRIDGE_DOWN) {

	    pline(pbuf, "drawbridge");

	} else if(IS_THRONE(typ)) {

	    pline(pbuf, defsyms[S_throne].explanation);
	    if (rnd(6) > 4)  {
		switch (rnd(13))  {
		    case 1:
			(void) adjattrib(rn2(A_MAX), -rn1(4,3), FALSE);
			losehp(rnd(10), "cursed throne", KILLED_BY_AN);
			break;
		    case 2:
			(void) adjattrib(rn2(A_MAX), 1, FALSE);
			break;
		    case 3:
		pline("A%s charge of electricity shoots through your body!",
			      (Shock_resistance) ? "" : " massive");
			if(Shock_resistance)
				losehp(rnd(6), "electric chair", KILLED_BY_AN);
			else	losehp(rnd(30), "electric chair", KILLED_BY_AN);
			exercise(A_CON, FALSE);
			break;
		    case 4:
			You("feel much, much better!");
			if(u.uhp >= (u.uhpmax - 5))  u.uhpmax += 4;
			u.uhp = u.uhpmax;
			make_blinded(0L,TRUE);
			make_sick(0L,FALSE);
			heal_legs();
			flags.botl = 1;
			break;
		    case 5:
			take_gold();
			break;
		    case 6:
			if(u.uluck + rn2(5) < 0) {
			    You("feel your luck is changing.");
			    change_luck(1);
			} else	    makewish();
			break;
		    case 7:
			{
			register int cnt = rnd(10);

			You("hear a voice echo:");
			verbalize("Thy audience hath been summoned, %s!",
				  flags.female ? "Dame" : "Sire");
			while(cnt--)
			    (void) makemon(courtmon(), u.ux, u.uy);
			break;
			}
		    case 8:
			You("hear a voice echo:");
			verbalize("By thy Imperious order, %s...",
				  flags.female ? "Dame" : "Sire");
			do_genocide(1);
			break;
		    case 9:
			You("hear a voice echo:");
	verbalize("A curse upon thee for sitting upon this most holy throne!");
			if (Luck > 0)  {
			    make_blinded(Blinded + rn1(100,250),TRUE);
			} else	    rndcurse();
			break;
		    case 10:
			if (Luck < 0 || (HSee_invisible & INTRINSIC))  {
				if (level.flags.nommap) {
					pline(
					"A terrible drone fills your head!");
					make_confused(HConfusion + rnd(30),
									FALSE);
				} else {
					pline("An image forms in your mind.");
					do_mapping();
				}
			} else  {
				Your("vision clarifies.");
				HSee_invisible |= FROMOUTSIDE;
				newsym(u.ux, u.uy);
			}
			break;
		    case 11:
			if (Luck < 0)  {
			    You("feel threatened.");
			    aggravate();
			} else  {

			    You("feel a wrenching sensation.");
			    tele();		/* teleport him */
			}
			break;
		    case 12:
			You("are granted a gift of insight!");
			if (invent) {
			    int ret, cval = rn2(5); /* agrees w/seffects() */
			    do {
				ret = ggetobj("identify", identify, cval);
			    } while (cval && (cval -= ret));
			}
			break;
		    case 13:
			Your("mind turns into a pretzel!");
			make_confused(HConfusion + rn1(7,16),FALSE);
			break;
		    default:	impossible("throne effect");
				break;
		}
	    } else	You("feel somehow out of place...");

	    if (!rn2(3) && IS_THRONE(levl[u.ux][u.uy].typ)) {
		/* may have teleported */
		pline("The throne vanishes in a puff of logic.");
		levl[u.ux][u.uy].typ = ROOM;
		if(Invisible) newsym(u.ux,u.uy);
	    }

#ifdef POLYSELF
	} else if (lays_eggs(uasmon) || u.umonnum == PM_QUEEN_BEE) {
		struct obj *uegg;

		if (!flags.female) {
			pline("Males can't lay eggs!");
			return 0;
		}

		if (u.uhunger < (int)objects[EGG].oc_nutrition) {
			You("are too weak to lay an egg.");
			return 0;
		}

		uegg = mksobj(EGG, FALSE, FALSE);
		uegg->spe = 1;
		uegg->quan = 1;
		uegg->owt = weight(uegg);
		uegg->corpsenm =
		    (u.umonnum==PM_QUEEN_BEE ? PM_KILLER_BEE : monsndx(uasmon));
		uegg->known = uegg->dknown = 1;
		You("lay an egg.");
		dropy(uegg);
		stackobj(uegg);
		morehungry((int)objects[EGG].oc_nutrition);
#endif
	} else if (u.uswallow)
		pline("There are no seats in here!");
	else
		pline("Having fun sitting on the floor?");
	return(1);
}

void
rndcurse()			/* curse a few inventory items at random! */
{
	int	nobj = 0;
	int	cnt, onum;
	struct	obj	*otmp;

	if(Antimagic) {
	    shieldeff(u.ux, u.uy);
	    You("feel a malignant aura surround you.");
	}

	for (otmp = invent; otmp; otmp = otmp->nobj)  nobj++;

	if (nobj)
	    for (cnt = rnd(6/((!!Antimagic) + (!!Half_spell_damage) + 1));
		 cnt > 0; cnt--)  {
		onum = rn2(nobj);
		for(otmp = invent; onum != 0; onum--)
		    otmp = otmp->nobj;

		if(otmp->oartifact && spec_ability(otmp, SPFX_INTEL) &&
		   rn2(10) < 8) {
		    pline("%s resists!", The(xname(otmp)));
		    continue;
		}

		if(otmp->blessed)
			unbless(otmp);
		else
			curse(otmp);
	    }
}

void
attrcurse()			/* remove a random INTRINSIC ability */
{
	switch(rnd(10)) {
	case 1 : if (HFire_resistance & INTRINSIC) {
			HFire_resistance &= ~INTRINSIC;
			You("feel warmer.");
			break;
		}
	case 2 : if (HTeleportation & INTRINSIC) {
			HTeleportation &= ~INTRINSIC;
			You("feel less jumpy.");
			break;
		}
	case 3 : if (HPoison_resistance & INTRINSIC) {
			HPoison_resistance &= ~INTRINSIC;
			You("feel a little sick!");
			break;
		}
	case 4 : if (HTelepat & INTRINSIC) {
			HTelepat &= ~INTRINSIC;
			if (Blind && !Telepat)
			    see_monsters();	/* Can't sense mons anymore! */
			Your("senses fail!");
			break;
		}
	case 5 : if (HCold_resistance & INTRINSIC) {
			HCold_resistance &= ~INTRINSIC;
			You("feel cooler.");
			break;
		}
	case 6 : if (HInvis & INTRINSIC) {
			HInvis &= ~INTRINSIC;
			You("feel paranoid.");
			break;
		}
	case 7 : if (HSee_invisible & INTRINSIC) {
			HSee_invisible &= ~INTRINSIC;
			You("thought you saw something!");
			break;
		}
	case 8 : if (Fast & INTRINSIC) {
			Fast &= ~INTRINSIC;
			You("feel slower.");
			break;
		}
	case 9 : if (Stealth & INTRINSIC) {
			Stealth &= ~INTRINSIC;
			You("feel clumsy.");
			break;
		}
	case 10: if (Protection & INTRINSIC) {
			Protection &= ~INTRINSIC;
			You("feel vulnerable.");
			break;
		}
	default: break;
	}
}

/*sit.c*/
