/* /domains/Horton/monsters/trail_bandit.c
   Trail bandit camping east of Horton. Level 6, SDC 65. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("trail bandit");
    set_id( ({ "trail bandit", "bandit", "raider", "human" }) );
    set_short("a trail bandit");
    set_long(
        "A lean human in patched armor with a vibro-knife and a scavenged\n"
        "laser pistol. Horton's militia keeps them off Main Street, so they\n"
        "work the scrub ridges and prey on travelers who leave the lights.\n"
    );

    set_race("human");
    set_class("fighter");
    set_level(6);
    set_alignment(-300);
    set_aggressive(1);
    set_gender("neutral");
    set_body_type("humanoid");

    set_stats("IQ", 10);
    set_stats("ME", 10);
    set_stats("MA", 8);
    set_stats("PS", 14);
    set_stats("PP", 14);
    set_stats("PE", 13);
    set_stats("PB", 7);
    set_stats("Spd", 16);

    set_max_hp(45);
    set_hp(45);
    set_stats("SDC", 65);
    set_stats("max_SDC", 65);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 45);
    set_stats("max_rifts_hp", 45);
}

int query_attack_damage() {
    return random(6) + 4;
}
