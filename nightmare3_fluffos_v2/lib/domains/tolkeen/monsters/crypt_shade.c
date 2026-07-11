/* /domains/tolkeen/monsters/crypt_shade.c */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("crypt shade");
    set_id( ({ "crypt shade", "shade", "spirit", "undead" }) );
    set_short("a crypt shade");
    set_long(
        "A thin shadow with too many angles. It whispers in a language\n"
        "that predates the Coalition - and maybe the Rifts.");

    set_race("undead");
    set_class("mage");
    set_level(5);
    set_exp(15000);
    set_alignment(-200);
    set_aggressive(1);
    set_gender("neuter");
    set_body_type("human");

    set_stats("IQ", 14);
    set_stats("ME", 16);
    set_stats("MA", 10);
    set_stats("PS", 8);
    set_stats("PP", 14);
    set_stats("PE", 12);
    set_stats("PB", 3);
    set_stats("Spd", 15);

    set_max_hp(30);
    set_hp(30);
    set_stats("MDC", 50);
    set_stats("max_MDC", 50);
    set_stats("is_MDC", 1);
    set_property("mdc_creature", 1);
    set_stats("rifts_hp", 30);
    set_stats("max_rifts_hp", 30);
}
