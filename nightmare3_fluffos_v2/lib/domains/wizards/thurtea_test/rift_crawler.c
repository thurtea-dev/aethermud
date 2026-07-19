/* /domains/wizards/thurtea_test/rift_crawler.c
   Low-threat Rifts MDC target: mega-damage hide, so it exercises the
   MDC path of the pooled damage model. Passive; attacks only when
   attacked. Same stat idiom as /domains/Praxis/monsters/brodkil.c. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("rift crawler");
    set_id( ({ "crawler", "rift crawler", "lesser rift crawler" }) );
    set_short("a leashed rift crawler");
    set_long(
        "A dog-sized scavenger from the far side of a rift, held on a\n"
        "dimensional tether. Its segmented hide sheds ordinary gunfire\n"
        "entirely; only mega-damage weapons mark it. It circles the limit\n"
        "of its leash with patient, unhurried steps. A tag on the tether\n"
        "reads TARGET UNIT 2: MEGA-DAMAGE.\n");
    set_race("rift crawler");
    set_level(3);
    set_exp(100);
    set_alignment(0);
    set_aggressive(0);
    set_stats("MDC", 30);
    set_stats("max_MDC", 30);
    set_stats("PS", 14);
    set_max_hp(60);
    set_hp(60);
    set_wimpy(0);
    set_property("position_str", "circles at the end of its tether.");
}
