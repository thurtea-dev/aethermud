/* /domains/Horton/areas/horton_quarry_rim.c
   Old quarry rim north of pine hollow. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Quarry Rim");
    set_long(
        "A cut in the hill exposes grey stone. The drop to the quarry floor\n"
        "is steep but climbable on a switchback path. A cabin sits east on\n"
        "the rim, smoke sometimes rising from its chimney.\n\n"
        "Pine hollow south. Quarry floor down. Hermit cabin east.");
    set_exits( ([
        "south" : "/domains/Horton/areas/horton_pine_hollow",
        "down"  : "/domains/Horton/areas/horton_quarry_floor",
        "east"  : "/domains/Horton/areas/horton_hermit_cabin"
    ]) );
    set_listen("default", "Wind across the cut. Loose gravel sliding.");
    set_smell("default", "Stone dust and woodsmoke.");
    set_items( ([
        "cut"   : "Pre-Rifts quarry. Abandoned mid-job.",
        "path"  : "Switchback down. Loose in places.",
        "cabin" : "Small, patched, lived-in."
    ]) );
}

void reset() { ::reset(); }
