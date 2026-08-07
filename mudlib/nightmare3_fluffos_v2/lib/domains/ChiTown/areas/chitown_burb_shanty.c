// /domains/ChiTown/areas/chitown_burb_shanty.c
// Shanty row west of the burb alley. Rest allowed.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("rest_allowed", 1);
    set_short("Shanty Row, The Burbs");
    set_long(
        "A row of patched-together shelters leaning against the outer wall\n"
        "district. Tarps, salvaged plating, and pre-Rifts doors form homes\n"
        "that were never meant to last. Fires burn in oil drums between the\n"
        "structures. Coalition patrols pass on the main street but rarely\n"
        "enter here.\n\n"
        "The dark alley lies east. A narrow path south leads deeper into\n"
        "the worst of the burbs. West opens into crowded tenements.");
    set_exits( ([
        "east"  : "/domains/ChiTown/areas/chitown_alley",
        "south" : "/domains/ChiTown/areas/chitown_burb_row",
        "west"  : "/domains/ChiTown/areas/burb_tenements"
    ]) );
    set_listen("default",
        "Children arguing, a distant patrol siren, and wood popping in barrel fires.");
    set_smell("default",
        "Smoke, unwashed bodies, and cooking oil.");
    set_items( ([
        "shelters" : "Patchwork homes built from whatever could be scavenged.",
        "fires" : "Oil-drum fires. The only heat most people here can afford.",
        "wall" : "The ferrocrete city wall looms beyond the shanties to the north."
    ]) );
}
