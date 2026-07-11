/* /domains/newcamelot/areas/castle_gatehouse.c
   Camelot Castle - Gatehouse. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Camelot Castle - Gatehouse");
    set_long(
        "The gatehouse of Camelot Castle. Twin drum towers flank a raised\n"
        "portcullis of mega-damage steel, and murder holes stare down from\n"
        "the vaulted ceiling. The royal gardens lie south. Beyond the gate\n"
        "to the north, the castle courtyard opens wide.");
    set_listen("default",
        "Wind humming through the firing ports.");
    set_smell("default",
        "Cold stone and gun oil.");
    set_items( ([
        "portcullis" : "MDC steel, raised. It could stop a tank shell.",
        "towers" : "Drum towers with arrow loops updated to firing ports.",
        "murder holes" : "You would rather not be under them when they are in use."
    ]) );
    set_exits( ([
        "south" : "/domains/newcamelot/areas/camelot_garden",
        "north" : "/domains/newcamelot/areas/castle_courtyard"
    ]) );
}

void reset() {
    ::reset();
    if(!present("royal knight guard", this_object()))
        new("/domains/newcamelot/npcs/royal_knight_guard")->move(this_object());
}
