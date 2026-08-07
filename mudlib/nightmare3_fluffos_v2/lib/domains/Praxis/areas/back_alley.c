// /d/Praxis/areas/back_alley.c
// Dark alley off Market Street: stub room hinting at black market.
// A concealed south exit leads to the deserter's hideout.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_short("Back Alley, off Market Street");
    set_long(
        "A narrow alley between two collapsed storefronts. Someone has\n"
        "strung salvaged cabling across the gap overhead to hang mismatched\n"
        "tarps that block the sky. The light is bad. Deliberately so.\n\n"
        "Scrawled on the crumbling wall in red spray-paint: ASK FOR CUTTER.\n"
        "Beneath the letters a crude arrow points west toward a half-open\n"
        "steel door set into the foundation wall.\n\n"
        "The alley continues south, narrowing between two building foundations.\n"
        "Market Street is back to the north. Cutter's is through the door to the west.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/market_street",
        "south" : "/domains/Praxis/areas/deserter_hideout",
        "west" : "/domains/Praxis/areas/cutters_shop"
    ]) );
    set_listen("default",
        "Dripping water somewhere, and the muffled noise of the market above.");
    set_smell("default",
        "Mildew, machine oil, and something chemical that doesn't have a name.");
    set_items( ([
        "wall" : "The graffiti is old but the paint is fresh in places. Someone still uses this alley.",
        "cabling" : "Improvised wiring looped overhead. Might be functional, might be an alarm.",
        "tarps" : "Heavy tarps cut visibility from above.",
        "spray" : "ASK FOR CUTTER. Whoever Cutter is, they want to be found, just not easily.",
        "graffiti": "Red letters: ASK FOR CUTTER. A crude arrow points west toward a steel door.",
        "door" : "A heavy steel door set into the foundation wall, propped half-open. The word CUTTER is scratched into the frame.",
        "arrow" : "A crude painted arrow pointing west. Someone added it recently."
    ]) );
}

void reset() {
    ::reset();
}
