// /domains/demon_plane/areas/demon_sliver_nest.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("demon_plane", 1);
    set_property("off_world", 1);
    set_short("Demon-Plane Sliver, Brood Nest");
    set_long(
        "Shredded armor and dried ichor litter a hollow in the basalt. Cracked\n"
        "helmets from a dozen worlds suggest this nest has fed for a long time.\n"
        "Eggs the size of footballs cluster in alcoves, pulsing faintly.");
    set_exits( ([
        "west" : "/domains/demon_plane/areas/demon_sliver_fissure"
    ]) );
}

void reset() {
    ::reset();
    if(!present("demon sliver stalker", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_stalker")->move(this_object());
    if(!present("demon sliver hunter", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_hunter")->move(this_object());
}
