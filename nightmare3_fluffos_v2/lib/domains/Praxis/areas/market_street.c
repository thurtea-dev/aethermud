// /d/Praxis/areas/market_street.c
// Market Street: east of Monument Square, lined with vendor stalls.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 1);
    set_short("Market Street");
    set_long(
        "A wide stretch of cracked asphalt that has been claimed by traders\n"
        "and vendors. Salvaged awnings and tarp lean-tos run the length of\n"
        "the street, shading stalls selling everything from weapons components\n"
        "to pre-Rifts magazines. The crowd is a mix of human survivors,\n"
        "D-Bees, and road-worn mercenaries.\n\n"
        "At the far east end, a reinforced storefront bears the handpainted\n"
        "sign GRIZ ARMS. To the south a narrower passage leads into Marta's\n"
        "general goods stall. A dark alley angles off between two collapsed\n"
        "buildings; it looks like it might lead somewhere interesting.\n\n"
        "Monument Square is back to the west.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/monument_square",
        "east" : "/domains/Praxis/areas/weapons_shop",
        "south" : "/domains/Praxis/areas/general_store",
        "alley" : "/domains/Praxis/areas/back_alley"
    ]) );
    set_listen("default",
        "Haggling, the scrape of cart wheels, someone demonstrating a weapon's sound effect, and music from somewhere.");
    set_smell("default",
        "Cooking meat, machine oil, the chemical tang of synthesized goods, and sweat.");
    set_items( ([
        "stalls" : "Rows of trade stalls. Someone has everything here, if you know where to look.",
        "awnings" : "Salvaged canvas and polymer sheeting stretched between poles and building walls.",
        "crowd" : "A moving mass of humans, D-Bees, and mercs, all ignoring each other carefully.",
        "alley" : "A dark gap between two collapsed buildings. Hard to see where it goes.",
        "sign" : "GRIZ ARMS, painted in red on a blast-rated storefront. A scrap of\n"
                 "paper is taped beneath it: 'Work and bounties inside. Ask for Griz.\n"
                 "Marta's stall is south. Say quest or work to get started.'",
    ]) );
}

void reset() {
    ::reset();
}
