// /domains/ChiTown/areas/cs_secure_wing.c
// Restricted CS officer wing above the barracks.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_property("cs_secure", 1);
    set_short("Chi-Town, CS Secure Wing");
    set_long(
        "A narrow corridor of reinforced ferrocrete behind a biometric airlock.\n"
        "Officer-only briefing rooms line one wall behind frosted glass. A wall\n"
        "display tracks active patrols and rift anomaly alerts across the sector.\n"
        "The atmosphere is quiet, controlled, and distinctly not for civilians.\n\n"
        "Stairs lead down to the barracks.");
    set_exits( ([
        "down" : "/domains/ChiTown/areas/cs_barracks"
    ]) );
    set_listen("default",
        "Muted ventilation and the faint tap of a status board updating.");
    set_smell("default",
        "Recycled air and cold metal.");
    set_items( ([
        "display" : "A patrol status board. Sector green. No active rift alerts.",
        "glass" : "Frosted briefing room windows. Voices are muffled inside.",
        "lock" : "Biometric scanners at every door. CS clearance required."
    ]) );
}
