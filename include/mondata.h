/*	SCCS Id: @(#)mondata.h	3.1	92/11/26	*/
/* Copyright (c) 1989 Mike Threepoint				  */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef MONDATA_H
#define MONDATA_H

#ifndef ALIGN_H
#include "align.h"
#endif

#define verysmall(ptr)		((ptr)->msize < MZ_SMALL)
#define bigmonst(ptr)		((ptr)->msize >= MZ_LARGE)

#define resists_fire(ptr)	(((ptr)->mresists & MR_FIRE) != 0)
#define resists_cold(ptr)	(((ptr)->mresists & MR_COLD) != 0)
#define resists_sleep(ptr)	(((ptr)->mresists & MR_SLEEP) != 0)
#define resists_disint(ptr)	(((ptr)->mresists & MR_DISINT) != 0)
#define resists_elec(ptr)	(((ptr)->mresists & MR_ELEC) != 0)
#define resists_poison(ptr)	(((ptr)->mresists & MR_POISON) != 0)
#define resists_acid(ptr)	(((ptr)->mresists & MR_ACID) != 0)
#define resists_ston(ptr)	(((ptr)->mresists & MR_STONE) != 0)

#define is_flyer(ptr)		(((ptr)->mflags1 & M1_FLY) != 0L)
#define is_floater(ptr)		((ptr)->mlet == S_EYE)
#define is_clinger(ptr)		(((ptr)->mflags1 & M1_CLING) != 0L)
#define is_swimmer(ptr)		(((ptr)->mflags1 & M1_SWIM) != 0L)
#define magic_breathing(ptr)	(((ptr)->mflags1 & M1_AMPHIBIOUS) != 0L)
#define passes_walls(ptr)	(((ptr)->mflags1 & M1_WALLWALK) != 0L)
#define amorphous(ptr)		(((ptr)->mflags1 & M1_AMORPHOUS) != 0L)
#define noncorporeal(ptr)	((ptr)->mlet == S_GHOST)
#define tunnels(ptr)		(((ptr)->mflags1 & M1_TUNNEL) != 0L)
#define needspick(ptr)		(((ptr)->mflags1 & M1_NEEDPICK) != 0L)
#define hides_under(ptr)	(((ptr)->mflags1 & M1_CONCEAL) != 0L)
#define is_hider(ptr)		(((ptr)->mflags1 & M1_HIDE) != 0L)
#define haseyes(ptr)		(((ptr)->mflags1 & M1_NOEYES) == 0L)
#define nohands(ptr)		(((ptr)->mflags1 & M1_NOHANDS) != 0L)
#define nolimbs(ptr)		(((ptr)->mflags1 & M1_NOLIMBS) == M1_NOLIMBS)
#define has_head(ptr)		(((ptr)->mflags1 & M1_NOHEAD) == 0L)
#define is_whirly(ptr)		((ptr)->mlet == S_VORTEX || \
				 (ptr) == &mons[PM_AIR_ELEMENTAL])
#define mindless(ptr)		(((ptr)->mflags1 & M1_MINDLESS) != 0L)
#define humanoid(ptr)		(((ptr)->mflags1 & M1_HUMANOID) != 0L)
#define is_animal(ptr)		(((ptr)->mflags1 & M1_ANIMAL) != 0L)
#define slithy(ptr)		(((ptr)->mflags1 & M1_SLITHY) != 0L)
#define is_meaty(ptr)		(!amorphous(ptr) && \
				 (ptr)->mlet != S_BLOB && \
				 (ptr)->mlet != S_JELLY && \
				 (ptr)->mlet != S_FUNGUS)
#define is_wooden(ptr)		((ptr) == &mons[PM_WOOD_GOLEM])
#define thick_skinned(ptr)	(((ptr)->mflags1 & M1_THICK_HIDE) != 0L)
#define lays_eggs(ptr)		(((ptr)->mflags1 & M1_OVIPAROUS) != 0L)
#define regenerates(ptr)	(((ptr)->mflags1 & M1_REGEN) != 0L)
#define perceives(ptr)		(((ptr)->mflags1 & M1_SEE_INVIS) != 0L)
#define can_teleport(ptr)	(((ptr)->mflags1 & M1_TPORT) != 0L)
#define control_teleport(ptr)	(((ptr)->mflags1 & M1_TPORT_CNTRL) != 0L)
#define telepathic(ptr)		((ptr) == &mons[PM_FLOATING_EYE] || \
				 (ptr) == &mons[PM_MIND_FLAYER])
#define is_armed(ptr)		attacktype(ptr, AT_WEAP)
#define acidic(ptr)		(((ptr)->mflags1 & M1_ACID) != 0L)
#define poisonous(ptr)		(((ptr)->mflags1 & M1_POIS) != 0L)
#define carnivorous(ptr)	(((ptr)->mflags1 & M1_CARNIVORE) != 0L)
#define herbivorous(ptr)	(((ptr)->mflags1 & M1_HERBIVORE) != 0L)
#define metallivorous(ptr)	(((ptr)->mflags1 & M1_METALLIVORE) != 0L)
#define polyok(ptr)		(((ptr)->mflags2 & M2_NOPOLY) == 0L)
#define is_undead(ptr)		(((ptr)->mflags2 & M2_UNDEAD) != 0L)
#define is_were(ptr)		(((ptr)->mflags2 & M2_WERE) != 0L)
#define is_elf(ptr)		(((ptr)->mflags2 & M2_ELF) != 0L)
#define is_dwarf(ptr)		(((ptr)->mflags2 & M2_DWARF) != 0L)
#define is_giant(ptr)		(((ptr)->mflags2 & M2_GIANT) != 0L)
#define is_golem(ptr)		((ptr)->mlet == S_GOLEM)
#define is_domestic(ptr)	(((ptr)->mflags2 & M2_DOMESTIC) != 0L)
#define is_orc(ptr)		(((ptr)->mflags2 & M2_ORC) != 0L)
#define is_human(ptr)		(((ptr)->mflags2 & M2_HUMAN) != 0L)
#define is_demon(ptr)		(((ptr)->mflags2 & M2_DEMON) != 0L)
#define is_mercenary(ptr)	(((ptr)->mflags2 & M2_MERC) != 0L)
#define is_male(ptr)		(((ptr)->mflags2 & M2_MALE) != 0L)
#define is_female(ptr)		(((ptr)->mflags2 & M2_FEMALE) != 0L)
#define is_neuter(ptr)		(((ptr)->mflags2 & M2_NEUTER) != 0L)
#define is_wanderer(ptr)	(((ptr)->mflags2 & M2_WANDER) != 0L)
#define always_hostile(ptr)	(((ptr)->mflags2 & M2_HOSTILE) != 0L)
#define always_peaceful(ptr)	(((ptr)->mflags2 & M2_PEACEFUL) != 0L)
#define extra_nasty(ptr)	(((ptr)->mflags2 & M2_NASTY) != 0L)
#define strongmonst(ptr)	(((ptr)->mflags2 & M2_STRONG) != 0L)
#  ifdef POLYSELF
#define can_breathe(ptr)	attacktype(ptr, AT_BREA)
#define cantwield(ptr)		(nohands(ptr) || verysmall(ptr))
#define cantweararm(ptr)	(breakarm(ptr) || sliparm(ptr))
#  endif /* POLYSELF */
#define throws_rocks(ptr)	(((ptr)->mflags2 & M2_ROCKTHROW) != 0L)
#define type_is_pname(ptr)	(((ptr)->mflags2 & M2_PNAME) != 0L)
#define is_lord(ptr)		(((ptr)->mflags2 & M2_LORD) != 0L)
#define is_prince(ptr)		(((ptr)->mflags2 & M2_PRINCE) != 0L)
#define is_ndemon(ptr)		(is_demon(ptr) && \
				 (((ptr)->mflags2 & (M2_LORD|M2_PRINCE)) == 0L))
#define is_dlord(ptr)		(is_demon(ptr) && is_lord(ptr))
#define is_dprince(ptr)		(is_demon(ptr) && is_prince(ptr))
#define is_minion(ptr)		((ptr)->mflags2 & M2_MINION)
#define is_lminion(ptr)		(is_minion(ptr) && \
				 (ptr)->maligntyp >= A_COALIGNED)
#define likes_gold(ptr)		(((ptr)->mflags2 & M2_GREEDY) != 0L)
#define likes_gems(ptr)		(((ptr)->mflags2 & M2_JEWELS) != 0L)
#define likes_objs(ptr)		(((ptr)->mflags2 & M2_COLLECT) != 0L || \
				 is_armed(ptr))
#define likes_magic(ptr)	(((ptr)->mflags2 & M2_MAGIC) != 0L)
#define is_longworm(ptr)	(((ptr) == &mons[PM_BABY_LONG_WORM]) || \
				 ((ptr) == &mons[PM_LONG_WORM]) || \
				 ((ptr) == &mons[PM_LONG_WORM_TAIL]))
#define is_covetous(ptr)	((ptr->mflags3 & M3_COVETOUS))
#define is_mplayer(ptr)		(((ptr) >= &mons[PM_ARCHEOLOGIST]) && \
				 ((ptr) <= &mons[PM_WIZARD]))
#define is_rider(ptr)		((ptr) == &mons[PM_DEATH] || \
				 (ptr) == &mons[PM_FAMINE] || \
				 (ptr) == &mons[PM_PESTILENCE])
#endif /* MONDATA_H */
