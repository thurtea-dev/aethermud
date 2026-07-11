// /domains/demon_plane/areas/demon_sliver_pit.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("demon_plane", 1);
    set_property("off_world", 1);
    set_short("Demon-Plane Sliver, Magma Pit");
    set_long(
        "You climb down beside the fissure into a bowl of cooled lava rock.\n"
        "Heat rises from cracks below. The walls are slick with condensation\n"
        "that smells like blood.");
    set_exits( ([
        "up" : "/domains/demon_plane/areas/demon_sliver_fissure"
    ]) );
}

void reset() {
    ::reset();
    if(!present("demon sliver hunter", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_hunter")->move(this_object());
}
