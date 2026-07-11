// /domains/Praxis/monsters/alvurron_overseer.c
// Brodkil overseer running the gargoyle empire's own captive hold. Aggressive.

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("brodkil overseer");
    set_id(({ "brodkil overseer", "overseer", "brodkil" }));
    set_short("a brodkil overseer");
    set_long(
        "A heavyset brodkil with bionic implants along one arm, a data slate\n"
        "in the other hand. It moves between the rows of cages with the bored\n"
        "efficiency of someone who has done this for a very long time. It does\n"
        "not look up from the slate unless you get close to the cages.");
    set_race("brodkil");
    set_level(10);
    set_exp(48000);
    set_alignment(-700);
    set_aggressive(1);
    set_stats("MDC", 200);
    set_stats("max_MDC", 200);
    set_stats("PS", 24);
    set_hp(220);
    set_property("mdc_creature", 1);
}
