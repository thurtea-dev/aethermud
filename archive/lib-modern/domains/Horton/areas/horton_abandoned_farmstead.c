/* /domains/Horton/areas/horton_abandoned_farmstead.c
   Collapsed farmhouse north of the wilderness trail. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Abandoned Farmstead");
    set_long(
        "A farmhouse stands half-collapsed, the roof caved in on one\n"
        "side. The barn behind it is a heap of charred timber. Whatever family\n"
        "lived here did not survive the Coming of the Rifts. Scavengers have\n"
        "picked most of it over, but the foundation looks undisturbed.");
    set_exits( ([
        "south" : "/domains/Horton/areas/horton_wilderness_trail"
    ]) );
    set_listen("default",
        "Creak of settling timber. Wind through broken walls. Silence from the barn.");
    set_smell("default",
        "Ash, rot, and old blood soaked into the floorboards.");
    set_items( ([
        "farmhouse"  : "Half-collapsed. The east wall still stands.",
        "barn"       : "Charred timber heap. Nothing salvageable left.",
        "foundation" : "Stone foundation under the farmhouse. Looks undisturbed.",
        "roof"       : "Caved in on the west side. Exposed rafters."
    ]) );
}

void reset() {
    object npc;
    object loot;

    if(!present("horton scavenger", this_object())) {
        npc = clone_object("/domains/Horton/monsters/horton_scavenger");
        npc->move(this_object());
    }
    if(!present("salvaged armor plate", this_object())) {
        loot = clone_object("/domains/Horton/equipment/salvaged_armor_plate");
        if(loot) loot->move(this_object());
    }
    ::reset();
}
