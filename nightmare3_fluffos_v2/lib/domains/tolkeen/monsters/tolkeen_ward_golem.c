/* /domains/tolkeen/monsters/tolkeen_ward_golem.c
   Crypt guardian. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("ward golem");
    set_id( ({ "ward golem", "golem", "guardian", "construct" }) );
    set_short("a ward golem");
    set_long(
        "A stone construct bound with glowing runes. It was built to keep\n"
        "things in the crypt - or keep the wrong people out.");

    set_race("golem");
    set_class("fighter");
    set_level(8);
    set_exp(30000);
    set_alignment(0);
    set_aggressive(1);
    set_gender("neuter");
    set_body_type("human");

    set_stats("IQ", 6);
    set_stats("ME", 6);
    set_stats("MA", 6);
    set_stats("PS", 22);
    set_stats("PP", 8);
    set_stats("PE", 20);
    set_stats("PB", 4);
    set_stats("Spd", 8);

    set_max_hp(40);
    set_hp(40);
    set_stats("MDC", 180);
    set_stats("max_MDC", 180);
    set_stats("is_MDC", 1);
    set_property("mdc_creature", 1);
    set_stats("rifts_hp", 40);
    set_stats("max_rifts_hp", 40);
}
