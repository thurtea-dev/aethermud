/* /domains/Praxis/monsters/rat.c
   Small prairie rat. Prey animal cloned by the trained hawk's hunt
   command (see /domains/Praxis/equipment/trained_hawk.c line 248).
   Deliberately harmless: not aggressive, minimal stats. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    int hp;

    ::create();

    hp = random(4) + 4;

    set_name("rat");
    set_id( ({ "rat", "prairie rat", "rodent", "animal", "prey" }) );
    set_short("a small prairie rat");
    set_long(
        "A small brown prairie rat, quick and nervous. It survives on the\n"
        "edges of Praxis by staying out of sight of anything larger than\n"
        "itself, which is nearly everything.");

    set_race("rat");
    set_class("animal");
    set_level(1);
    set_exp(50);
    set_alignment(0);
    set_aggressive(0);

    set_stats("IQ", 2);
    set_stats("ME", 4);
    set_stats("MA", 3);
    set_stats("PS", 3);
    set_stats("PP", 12);
    set_stats("PE", 8);
    set_stats("PB", 4);
    set_stats("Spd", 14);

    set_stats("rifts_hp", hp);
    set_stats("max_rifts_hp", hp);
    set_stats("SDC", 3);
    set_stats("max_SDC", 3);
    set_stats("MDC", 0);
    set_stats("max_MDC", 0);
    set_stats("is_MDC", 0);

    setenv("rifts_occ", "animal");
    setenv("rifts_flags", "");

    set_hp(hp);
    set_mp(0);
    set_sp(20);

    set_body_type("animal");
}
