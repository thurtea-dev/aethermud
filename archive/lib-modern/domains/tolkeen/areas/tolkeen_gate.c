// /domains/tolkeen/areas/tolkeen_gate.c
// Tolkeen domain stub: fortified entry for future expansion.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_property("domain_stub", 1);
    set_short("Tolkeen, Mage Gate");
    set_long(
        "A rune-etched arch of fused stone marks the border of the Free City of\n"
        "Tolkeen. Techno-wizard sigils pulse along the frame. Beyond the gate,\n"
        "smoke and ley-light rise from a city that refuses Coalition rule.\n"
        "This zone is a stub for future content.");
    set_exits( ([
        "north" : "/domains/tolkeen/areas/tolkeen_square"
    ]) );
    set_listen("default", "Distant spell thunder and the hum of active wards.");
    set_smell("default", "Ozone, incense, and forge smoke.");
    set_items( ([
        "arch" : "Techno-wizard runes crawl across the gate frame.",
        "sigils" : "They flare when Coalition forces approach, or so the stories say."
    ]) );
}
