// /domains/ChiTown/areas/cs_command_plaza.c
// CS Command Plaza. Interior hub north of the main boulevard.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 1);
    set_short("Chi-Town, CS Command Plaza");
    set_long(
        "A broad ferrocrete plaza inside the Coalition wall. Gray banners\n"
        "snap in the filtered city wind. A polished CS skull emblem is set\n"
        "into the center of the ground in inlaid steel. Military traffic\n"
        "moves in disciplined lines between hardened buildings on every side.\n\n"
        "The vendor district lies north. The main boulevard is south. Barracks\n"
        "stand to the east. The armory lies west. An ISS checkpoint opens\n"
        "northeast into restricted posts.");
    set_exits( ([
        "south" : "/domains/ChiTown/areas/chitown_boulevard",
        "north" : "/domains/ChiTown/areas/chitown_officer_row",
        "east"  : "/domains/ChiTown/areas/cs_barracks",
        "west"  : "/domains/ChiTown/areas/cs_armory",
        "northeast" : "/domains/ChiTown/areas/cs_iss_checkpoint"
    ]) );
    set_listen("default",
        "Boots on ferrocrete, clipped orders, and the distant hum of patrol drones.");
    set_smell("default",
        "Gun oil, polished metal, and the sterile tang of filtered city air.");
    set_items( ([
        "plaza" : "Broad ferrocrete worn smooth by military traffic.",
        "emblem" : "A CS skull inlaid in steel at the center of the plaza.",
        "banners" : "Gray Coalition banners on every pole. HUMANITY FIRST.",
        "buildings" : "Hardened CS structures. Barracks east. Armory west."
    ]) );
}
