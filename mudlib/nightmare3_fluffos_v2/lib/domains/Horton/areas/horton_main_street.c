/* /domains/Horton/areas/horton_main_street.c
   Horton Main Street -- center of the small friendly town. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Horton Main Street");
    set_long(
        "Prefab buildings and salvaged pre-Rifts storefronts line a wide dirt\n"
        "street. Friendly faces, unusual for post-Rifts Earth. D-Bees and\n"
        "humans share the street without incident, trading goods and conversation\n"
        "with easy familiarity. Children play between the buildings. Someone is\n"
        "cooking something that smells good.\n\n"
        "A general store sits to the west. The inn is east. The hovertrain\n"
        "station is south. Forest starts beyond the northern buildings.");
    set_exits( ([
        "north" : "/domains/Horton/areas/horton_forest_edge",
        "east"  : "/domains/Horton/areas/horton_inn",
        "west"  : "/domains/Horton/areas/horton_store",
        "south" : "/domains/Horton/areas/horton_station"
    ]) );
    set_listen("default",
        "Children, conversation, and the distant sound of someone chopping wood.");
    set_smell("default",
        "Cooking food, sawdust, and fresh air. Nothing chemical. Nothing military.");
    set_items( ([
        "buildings" : "Prefab and salvaged, but solid. People here take care "
                      "of their things.",
        "children"  : "Kids playing. You have not seen this in a while.",
        "street"    : "Packed dirt, well-worn. The town is small but active.",
        "faces"     : "People look at you with curiosity, not hostility. "
                      "That takes getting used to.",
        "forest"    : "The treeline is visible to the north, massive and green."
    ]) );
}

void reset() { ::reset(); }
