/* /domains/Horton/areas/horton_forest_edge.c
   Horton Forest Edge -- where the town gives way to Rifts-mutated wilderness. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Horton Forest Edge");
    set_long(
        "The town gives way to massive Rifts-mutated trees. The trunks are\n"
        "twice the width of a man, bark black and glistening. Strange sounds\n"
        "echo from deeper in, something large moving, or something trying\n"
        "to sound large. The townsfolk warn visitors not to wander in alone.\n"
        "They say this quietly, with the weight of people who have lost someone.");
    set_exits( ([
        "south" : "/domains/Horton/areas/horton_main_street",
        "north" : "/domains/Horton/areas/horton_forest_deep",
        "west"  : "/domains/Horton/areas/horton_outskirts",
        "east"  : "/domains/Horton/areas/horton_logging_road"
    ]) );
    set_listen("default",
        "Wind in massive branches. Something moving in the underbrush. Silence.");
    set_smell("default",
        "Wet earth, pine resin, and something animal.");
    set_items( ([
        "trees"      : "Enormous. Rifts-mutated. The bark has an oily sheen.",
        "underbrush" : "Dense and tangled. Something was in it recently.",
        "town"       : "Visible to the south. Safe, relatively speaking.",
        "sounds"     : "Better not to think too hard about what makes those sounds."
    ]) );
}

void reset() {
    object wolf;
    if(random(3) == 0 && !present("plains wolf", this_object())) {
        wolf = clone_object("/domains/Praxis/monsters/plains_wolf");
        wolf->move(this_object());
    }
    ::reset();
}
