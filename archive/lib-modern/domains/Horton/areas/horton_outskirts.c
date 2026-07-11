/* /domains/Horton/areas/horton_outskirts.c
   Horton outskirts watchtower at the wilderness perimeter. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Horton Outskirts");
    set_long(
        "A sandbagged watchtower marks the edge of Horton proper.\n"
        "Militia volunteers keep watch over the treeline with scavenged rifles\n"
        "and a single working spotlight. Beyond the perimeter, the wilderness\n"
        "offers no protection.");
    set_exits( ([
        "east"  : "/domains/Horton/areas/horton_forest_edge",
        "north" : "/domains/Horton/areas/horton_wilderness_trail"
    ]) );
    set_listen("default",
        "Wind across open ground. A spotlight motor humming. Distant forest sounds.");
    set_smell("default",
        "Dust, gun oil, and pine from the treeline.");
    set_items( ([
        "watchtower" : "Sandbagged timber and salvaged sheet metal. Functional.",
        "spotlight"  : "One working spotlight sweeps the treeline at intervals.",
        "sandbags"   : "Stacked high enough to stop small arms fire.",
        "treeline"   : "Dark forest to the north and east. Horton lies east."
    ]) );
}

void reset() {
    object npc;

    if(!present("horton militia guard", this_object())) {
        npc = clone_object("/domains/Horton/monsters/horton_militia_guard");
        npc->move(this_object());
    }
    ::reset();
}
