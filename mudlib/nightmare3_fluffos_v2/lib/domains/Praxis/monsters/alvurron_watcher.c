// /domains/Praxis/monsters/alvurron_watcher.c
// Gargoyle watcher posted at the Alvurron plaza. Non-aggressive unless attacked.

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("gargoyle watcher");
    set_id(({ "gargoyle watcher", "watcher", "gargoyle" }));
    set_short("a gargoyle watcher");
    set_long(
        "A gargoyle perched motionless on a high ledge, wings folded, yellow\n"
        "eyes tracking everything that crosses the plaza below. It does not\n"
        "interfere with the crowd. It is simply watching, and cataloguing,\n"
        "and waiting.");
    set_race("gargoyle");
    set_level(8);
    set_exp(32000);
    set_alignment(-500);
    set_aggressive(0);
    set_stats("MDC", 150);
    set_stats("max_MDC", 150);
    set_stats("PS", 20);
    set_hp(160);
    set_property("mdc_creature", 1);
    set_property("flight_capable", 1);
}
