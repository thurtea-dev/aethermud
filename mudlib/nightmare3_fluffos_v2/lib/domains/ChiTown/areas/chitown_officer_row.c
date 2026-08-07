// /domains/ChiTown/areas/chitown_officer_row.c
// Officer housing row north of the CS command plaza.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Chi-Town, Officer Row");
    set_long(
        "A clean ferrocrete street reserved for Coalition officers and\n"
        "trusted citizens. Apartment blocks line both sides in identical\n"
        "gray modules. Propaganda panels display CS victories and human\n"
        "supremacy slogans. Patrol drones hover at regular intervals.\n\n"
        "The vendor market lies north. The command plaza is south. A public\n"
        "propaganda hall opens west.");
    set_exits( ([
        "north" : "/domains/ChiTown/areas/chitown_market",
        "south" : "/domains/ChiTown/areas/cs_command_plaza",
        "west"  : "/domains/ChiTown/areas/cs_propaganda_hall"
    ]) );
    set_listen("default",
        "Muted footsteps, drone hum, and Coalition anthem from distant speakers.");
    set_smell("default",
        "Filtered air and disinfectant.");
    set_items( ([
        "modules" : "Identical gray housing blocks. Every window has a CS decal.",
        "panels" : "Propaganda screens. HUMANITY FIRST. THE STRONG SURVIVE.",
        "drones" : "Patrol drones on fixed routes. They record everything."
    ]) );
}
