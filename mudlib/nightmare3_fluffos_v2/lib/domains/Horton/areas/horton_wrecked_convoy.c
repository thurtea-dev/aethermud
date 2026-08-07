/* /domains/Horton/areas/horton_wrecked_convoy.c
   Burned convoy west of the abandoned farmstead. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Wrecked Convoy");
    set_long(
        "Three trucks burned to frames on a dirt road that no longer goes\n"
        "anywhere. Bullet holes stitch the cab doors. Someone piled bones\n"
        "under the middle chassis - animals, or not.\n\n"
        "Farmstead east. Pine hollow north.");
    set_exits( ([
        "east"  : "/domains/Horton/areas/horton_abandoned_farmstead",
        "north" : "/domains/Horton/areas/horton_pine_hollow"
    ]) );
    set_listen("default", "Metal ticking as it cools in the wind.");
    set_smell("default", "Old fire and oil.");
    set_items( ([
        "trucks" : "Pre-Rifts civilian haulers. Stripped of engines.",
        "holes"  : "Small arms and something heavier.",
        "bones"  : "You decide not to sort them carefully."
    ]) );
}

void reset() {
    ::reset();
    if(!present("horton scavenger", this_object()))
        clone_object("/domains/Horton/monsters/horton_scavenger")->move(this_object());
}
