// /domains/Praxis/monsters/sea_serpent.c
// Dimensional sea serpent at the Splynn ocean rift.

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("sea serpent");
    set_id(({ "serpent", "sea serpent", "beast" }));
    set_short("a dimensional sea serpent");
    set_long(
        "A scaled coil as thick as a tree trunk breaks the warped water.\n"
        "Eyes like molten amber. It came through the rift and has not left.");
    set_race("monster");
    set_level(8);
    set_exp(35000);
    set_alignment(-400);
    set_aggressive(1);
    set_stats("MDC", 120);
    set_stats("max_MDC", 120);
    set_hp(180);
}
