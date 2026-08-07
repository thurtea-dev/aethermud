/* /domains/Horton/areas/horton_quarry_floor.c
   Floor of the abandoned quarry. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Quarry Floor");
    set_long(
        "Broken stone and standing water. Tool sheds collapsed into heaps.\n"
        "Scratch marks climb the walls in places no human would climb for\n"
        "fun. The rim path leads up.\n\n"
        "Quarry rim is up.");
    set_exits( ([
        "up" : "/domains/Horton/areas/horton_quarry_rim"
    ]) );
    set_listen("default", "Drip. Echo. A soft click on stone.");
    set_smell("default", "Cold water and animal musk.");
    set_items( ([
        "water"  : "Rain pooled in the cut. Opaque.",
        "marks"  : "Claw or tool - hard to tell from here.",
        "sheds"  : "Nothing left but warped tin."
    ]) );
}

void reset() {
    ::reset();
    if(!present("quarry lurker", this_object()))
        clone_object("/domains/Horton/monsters/quarry_lurker")->move(this_object());
}
