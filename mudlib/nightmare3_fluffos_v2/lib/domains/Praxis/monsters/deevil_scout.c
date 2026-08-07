// /domains/Praxis/monsters/deevil_scout.c
// Deevil scout. Cunning and cruel.

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("deevil scout");
    set_id(({ "deevil", "scout", "deevil scout" }));
    set_short("a deevil scout");
    set_long(
        "A red-skinned deevil with horns and a forked tail. It moves with\n"
        "unnatural grace and watches everything with cold intelligence.");
    set_race("deevil");
    set_level(8);
    set_exp(35000);
    set_alignment(-700);
    set_aggressive(1);
    set_stats("MDC", 60);
    set_stats("max_MDC", 60);
    set_stats("ME", 16);
    set_hp(150);
}
