/* /domains/newcamelot/areas/academy_lecture_hall.c
   Royal Academy - Lecture Hall. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Royal Academy - Lecture Hall");
    set_long(
        "A steep bowl of benches descends to a lectern ringed by chalk\n"
        "circles. Equations of PPE flow and ley-line harmonics cover three\n"
        "blackboards, one of which is erasing itself and starting over\n"
        "with visible impatience. The quad is south.");
    set_listen("default",
        "Chalk squeaking. No hand holds it.");
    set_smell("default",
        "Chalk dust and old varnish.");
    set_items( ([
        "blackboards" : "Ley-line harmonics on the first two. The third is sulking.",
        "circles" : "Containment chalk, scuffed by demonstration.",
        "lectern" : "Scorched on one corner. There is a story there."
    ]) );
    set_exits( ([
        "south" : "/domains/newcamelot/areas/academy_quad"
    ]) );
}

void reset() {
    ::reset();
    if(!present("headmaster", this_object()))
        new("/domains/newcamelot/npcs/academy_headmaster")->move(this_object());
}
