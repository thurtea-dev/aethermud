// /domains/Praxis/monsters/alvurron_trader.c
// Gargoyle trader working the Alvurron market row. Non-aggressive.

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("gargoyle trader");
    set_id(({ "gargoyle trader", "trader", "gargoyle" }));
    set_short("a gargoyle trader");
    set_long(
        "A gargoyle in a leather harness hung with pouches and tally cords,\n"
        "standing behind a stall of salvage and dimensional oddities. It\n"
        "sizes you up the way a merchant sizes up anyone, then goes back to\n"
        "arranging its goods.");
    set_race("gargoyle");
    set_level(7);
    set_exp(28000);
    set_alignment(-300);
    set_aggressive(0);
    set_stats("MDC", 120);
    set_stats("max_MDC", 120);
    set_stats("PS", 18);
    set_hp(140);
    set_property("mdc_creature", 1);
    set_property("flight_capable", 1);
}
