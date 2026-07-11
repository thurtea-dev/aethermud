// /domains/newcamelot/areas/europe_road.c
// Rolling hills south of New Camelot gate.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Europe Road, Southern Approach");
    set_long(
        "Rolling hills and dense forest stretch south as far as you can see.\n"
        "The road here is ancient cobblestone, worn smooth by centuries of\n"
        "travel. Wildflowers grow in the cracks between stones. The air\n"
        "smells of rain and green growing things.\n\n"
        "The spires of New Camelot are visible to the north behind you.");
    set_exits( ([
        "north" : "/domains/newcamelot/areas/camelot_gate",
        "south" : "/domains/newcamelot/areas/black_forest",
        "west"  : "/domains/Praxis/areas/long_road",
        "east"  : "/domains/NGR/areas/NGR_gate"
    ]) );
    set_listen("default",
        "Wind through the forest. Birdsong. The distant bells of New Camelot.");
    set_smell("default",
        "Rain, green grass, wildflowers, and old stone.");
    set_items( ([
        "road" : "Ancient cobblestone. Older than memory.",
        "hills" : "Rolling green hills stretching south. Peaceful.",
        "forest" : "Dense old-growth forest lining the road.",
        "flowers" : "Wildflowers growing in the cracks of the old road.",
        "spires" : "The towers of New Camelot, visible to the north."
    ]) );
}

void reset() { ::reset(); }
