/* /domains/newcamelot/areas/academy_dormitory.c
   Royal Academy - Dormitory. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Royal Academy - Dormitory");
    set_long(
        "The student dormitory of the Royal Academy. Bunks are crammed\n"
        "under sloping eaves, every flat surface buried in notes, crystal\n"
        "foci, and half-finished essays. A kettle heats itself in the\n"
        "corner without benefit of fire. The quad is east.");
    set_listen("default",
        "The kettle beginning to sing.");
    set_smell("default",
        "Tea, ink, and singed wool.");
    set_items( ([
        "bunks" : "Narrow, lofted, and hung with privacy curtains that occasionally soundproof themselves.",
        "kettle" : "It whistles when watched, on principle.",
        "notes" : "Cramming for the summoning practical, by the look."
    ]) );
    set_exits( ([
        "east" : "/domains/newcamelot/areas/academy_quad"
    ]) );
}

void reset() {
    ::reset();
    if(!present("academy student", this_object()))
        new("/domains/newcamelot/npcs/academy_student")->move(this_object());
}
