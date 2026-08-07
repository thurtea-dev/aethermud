/* /domains/Horton/areas/horton_marsh_edge.c
   Wetland fringe west of the creek. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Marsh Edge");
    set_long(
        "Standing water and cattails. Blackflies rise in clouds. A few\n"
        "rotting fence posts mark where a pasture used to end. The ground\n"
        "sucks at boots.\n\n"
        "Creek crossing is east. Overgrown orchard south.");
    set_exits( ([
        "east"  : "/domains/Horton/areas/horton_creek_crossing",
        "south" : "/domains/Horton/areas/horton_overgrown_orchard"
    ]) );
    set_listen("default", "Insects. A splash that might be a fish.");
    set_smell("default", "Stagnant water and sweet rot.");
    set_items( ([
        "cattails" : "Tall and brown. Good cover for anything patient.",
        "posts"    : "Old pasture line. Wire long gone.",
        "water"    : "Knee-deep in places. Do not trust the bottom."
    ]) );
}

void reset() {
    ::reset();
    if(random(2) == 0 && !present("feral boar", this_object()))
        clone_object("/domains/Horton/monsters/feral_boar")->move(this_object());
}
