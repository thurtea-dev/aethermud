// /domains/chitown/areas/illinois_road.c
// Open wasteland south of Chi-Town gate.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Illinois Road, Southern Approach");
    set_long(
        "What was once a highway stretches south across the open wastes.\n"
        "Rusted pre-Rifts road signs lean at odd angles, their text long\n"
        "since faded. The great wall of Chi-Town looms behind you to the\n"
        "north. Beyond it, nothing but open wasteland and the occasional\n"
        "blasted tree.");
    set_exits( ([
        "north" : "/domains/chitown/areas/chitown_burbs",
        "east"  : "/domains/chitown/areas/bandit_camp",
        "west"  : "/domains/Praxis/areas/old_highway",
        "south" : "/domains/PuertoAngel/areas/pa_outskirts"
    ]) );
    set_listen("default",
        "Wind over cracked asphalt. Distant Coalition turrets humming.");
    set_smell("default",
        "Dust, old asphalt, and the faint chemical smell drifting from the city.");
    set_items( ([
        "signs" : "Old road signs. Too faded to read.",
        "highway" : "Cracked asphalt stretching south into the wastes.",
        "wastes" : "Open, flat, and largely dead. The sky is a washed-out gray.",
        "wall" : "The immense Chi-Town wall to the north."
    ]) );
}

void reset() { ::reset(); }
