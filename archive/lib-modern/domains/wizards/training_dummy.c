/* /domains/wizards/training_dummy.c
   SDC humanoid training dummy for the combat testing zone.
   Passive -- does not fight back. Respawns via room reset. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("training dummy");
    set_id( ({ "training dummy", "dummy", "humanoid dummy", "sdc dummy" }) );
    set_short("an SDC humanoid training dummy");
    set_long(
        "A padded humanoid figure on a weighted base, built to absorb weapon\n"
        "impacts during testing. SDC construction. Only SDC weapons register\n"
        "meaningful damage against it. It does not fight back.\n"
    );
    set_race("human");
    set_level(1);
    set_alignment(0);
    set_max_hp(150);
    set_hp(150);
    set_stats("strength", 8);
    set_stats("dexterity", 1);
    set_stats("constitution", 10);
    set_stats("rifts_hp", 150);
    set_property("position_str", "stands motionless at the first target station.");
    set_property("aggressive", 0);
    set_property("passive", 1);
    set_property("no_xp", 1);
    set_property("no_loot", 1);
    set_property("mdc_creature", 0);
    set_stats("MDC", 0);
    set_stats("max_MDC", 0);
}
