/* /domains/Horton/areas/horton_hermit_cabin.c
   Hermit cabin on the quarry rim. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Hermit Cabin");
    set_long(
        "One room: bunk, stove, shelves of jars and ammo. Maps of the\n"
        "Horton woods pinned to the wall with knife points. The door faces\n"
        "the quarry rim.\n\n"
        "Quarry rim is west.");
    set_exits( ([
        "west" : "/domains/Horton/areas/horton_quarry_rim"
    ]) );
    set_listen("default", "Stove tick. Wind under the door.");
    set_smell("default", "Woodsmoke, coffee, and gun oil.");
    set_items( ([
        "maps"   : "Hand-drawn. X marks near the quarry floor and deep forest.",
        "jars"   : "Dried meat, beans, something pickled.",
        "shelves": "Practical clutter. No waste."
    ]) );
}

void reset() {
    ::reset();
    if(!present("horton hermit", this_object()))
        clone_object("/domains/Horton/npcs/horton_hermit")->move(this_object());
}
