// /d/Praxis/areas/splynn/preserve_thicket.c
// Splynn Preserves hunting loop, R11: Bramble Deep, a spur off the thornbrake.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Bramble Deep");
    set_long(
        "The thornbrake stops being passable in any normal sense here. The\n"
        "black spines have grown into and through each other for years\n"
        "uncounted, knotting into a low, dark tunnel just wide enough for a\n"
        "crouched body. Little light reaches the ground. What does is broken\n"
        "into hard-edged slivers by the canopy of thorn overhead.\n\n"
        "Something has been through recently. The gaps here are too clean to\n"
        "be natural, cut or forced by a body stronger than the wood. The\n"
        "only easy way out is back east.");
    set_exits( ([
        "east" : "/domains/Praxis/areas/splynn/preserve_thornbrake"
    ]) );
    set_listen("default",
        "Thorns scraping against something with a hide too thick to care,\n"
        "somewhere ahead in the dark of the tunnel.");
    set_smell("default",
        "Crushed green wood, old sap gone sour, and a musk heavier than\n"
        "anything smelled in the open brake.");
    set_items( ([
        "tunnel"  : "A low tunnel forced through the thornbrush, the spines\n"
                    "bent and broken rather than grown that way. Something big\n"
                    "made this.",
        "slivers" : "Thin blades of daylight breaking through the thorn canopy\n"
                    "overhead, just enough to see by.",
        "spines"  : "Black thorn spines knotted into an almost solid wall on\n"
                    "every side but the tunnel itself."
    ]) );
}

void reset() {
    ::reset();
    if(random(10) < 7 && !present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
    if(random(10) < 3 && !present("xiticix warrior", this_object()))
        clone_object(MONSTER_PATH+"xiticix_warrior.c")->move(this_object());
}
