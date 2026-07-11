// /domains/demon_plane/areas/demon_sliver_scar.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("demon_plane", 1);
    set_property("off_world", 1);
    set_short("Demon-Plane Sliver, Ley Scar Edge");
    set_long(
        "The demon-plane sky opens here onto a visible ley scar: a ribbon of\n"
        "violet energy arcing toward the rift mouth. Standing at the edge you\n"
        "can still feel Praxis on the other side, impossibly far and close at once.");
    set_exits( ([
        "west" : "/domains/demon_plane/areas/demon_sliver_gate",
        "north" : "/domains/demon_plane/areas/demon_sliver_depths"
    ]) );
}

void reset() {
    ::reset();
    if(!present("demon sliver stalker", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_stalker")->move(this_object());
}
