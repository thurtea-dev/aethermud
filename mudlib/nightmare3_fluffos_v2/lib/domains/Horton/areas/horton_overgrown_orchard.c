/* /domains/Horton/areas/horton_overgrown_orchard.c
   Dead orchard west of the outskirts watchtower. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Overgrown Orchard");
    set_long(
        "Rows of apple trees gone wild, trunks twisted, fruit small and\n"
        "bitter. Wasps work the fallen crop. The outskirts watchtower is\n"
        "visible through the branches to the east.\n\n"
        "Outskirts east. Marsh edge north.");
    set_exits( ([
        "east"  : "/domains/Horton/areas/horton_outskirts",
        "north" : "/domains/Horton/areas/horton_marsh_edge"
    ]) );
    set_listen("default", "Wasps. Wind in dry leaves.");
    set_smell("default", "Fermenting fruit and woodsmoke from town.");
    set_items( ([
        "trees" : "Unpruned for decades. Some still bear fruit.",
        "fruit" : "Hard, sour, and half-eaten by insects.",
        "wasps" : "They own this place in late summer."
    ]) );
}

void reset() {
    ::reset();
    if(!present("wild dog pack", this_object()))
        clone_object("/domains/Horton/monsters/wild_dog_pack")->move(this_object());
}
