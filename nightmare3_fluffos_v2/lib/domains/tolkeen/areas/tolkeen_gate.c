// /domains/tolkeen/areas/tolkeen_gate.c
// Free City of Tolkeen - mage gate entry.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Mage Gate");
    set_long(
        "A rune-etched arch of fused stone marks the border of the Free City\n"
        "of Tolkeen. Techno-wizard sigils pulse along the frame. Beyond the\n"
        "gate, smoke and ley-light rise from a city that refuses Coalition\n"
        "rule.\n\n"
        "Liberation Square is north. The southern approach road leads toward\n"
        "Coalition-watched wilds.");
    set_exits( ([
        "north" : "/domains/tolkeen/areas/tolkeen_square",
        "south" : "/domains/tolkeen/areas/tolkeen_approach"
    ]) );
    set_listen("default", "Distant spell thunder and the hum of active wards.");
    set_smell("default", "Ozone, incense, and forge smoke.");
    set_items( ([
        "arch" : "Techno-wizard runes crawl across the gate frame.",
        "sigils" : "They flare when Coalition forces approach, or so the stories say."
    ]) );
}

void reset() {
    ::reset();
    if(!present("tolkeen gate guard", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_gate_guard")->move(this_object());
}
