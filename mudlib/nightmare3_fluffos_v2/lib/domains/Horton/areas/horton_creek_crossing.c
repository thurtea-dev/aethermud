/* /domains/Horton/areas/horton_creek_crossing.c
   Shallow creek west of the wilderness trail. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Creek Crossing");
    set_long(
        "A shallow creek cuts the trail. Stepping stones and a half-rotted\n"
        "log make a crossing. Mud on both banks shows boot prints and hoof\n"
        "marks. Marsh grass thickens to the west.\n\n"
        "The wilderness trail is east. Marsh edge west.");
    set_exits( ([
        "east" : "/domains/Horton/areas/horton_wilderness_trail",
        "west" : "/domains/Horton/areas/horton_marsh_edge"
    ]) );
    set_listen("default", "Water over stone. Frogs. Something heavy in the reeds.");
    set_smell("default", "Wet mud and crushed mint.");
    set_items( ([
        "creek"  : "Clear enough to see minnows. Cold.",
        "stones" : "Flat and slick. Use them or get wet.",
        "prints" : "Human boots and something cloven."
    ]) );
}

void reset() {
    ::reset();
    if(!present("feral boar", this_object()))
        clone_object("/domains/Horton/monsters/feral_boar")->move(this_object());
}
