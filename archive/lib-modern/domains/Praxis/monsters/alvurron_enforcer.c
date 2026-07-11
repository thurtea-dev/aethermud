// /domains/Praxis/monsters/alvurron_enforcer.c
// Brodkil enforcer guarding the Alvurron inner district. Aggressive.

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("brodkil enforcer");
    set_id(({ "brodkil enforcer", "enforcer", "brodkil" }));
    set_short("a brodkil enforcer");
    set_long(
        "A brodkil built for one purpose, layered in scavenged bionic plating\n"
        "and old battle scars. It moves with deliberate, unhurried slowness,\n"
        "the way something moves when it has never lost a fight. It is not\n"
        "interested in talking.");
    set_race("brodkil");
    set_level(14);
    set_exp(85000);
    set_alignment(-800);
    set_aggressive(1);
    set_stats("MDC", 320);
    set_stats("max_MDC", 320);
    set_stats("PS", 30);
    set_hp(320);
    set_property("mdc_creature", 1);
}
