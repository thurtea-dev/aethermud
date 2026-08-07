// /domains/Praxis/monsters/minor_demon.c
// Lesser demon from a rift incursion.

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("lesser demon");
    set_id(({ "demon", "lesser demon", "fiend" }));
    set_short("a lesser demon");
    set_long(
        "A twisted humanoid form wreathed in sulfurous smoke. Clawed hands\n"
        "and too many teeth. It hungered through the rift and has not fed yet.");
    set_race("demon");
    set_level(6);
    set_exp(22000);
    set_alignment(-900);
    set_aggressive(1);
    set_stats("MDC", 50);
    set_stats("max_MDC", 50);
    set_hp(120);
}
