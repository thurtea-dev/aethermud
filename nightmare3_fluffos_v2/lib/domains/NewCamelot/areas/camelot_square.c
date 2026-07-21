// /domains/NewCamelot/areas/camelot_square.c
// Main plaza of New Camelot. Moxim spawns here.

#include <std.h>
#include <rooms.h>

#define MOXIM_PATH "/domains/Praxis/monsters/moxim"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 1);
    set_short("New Camelot, Main Plaza");
    set_long(
        "The heart of New Camelot. A replica Round Table sits at the center\n"
        "of the plaza as a monument to the Camelot Order's founding ideals.\n"
        "Market stalls line the edges selling goods from across Europe.\n"
        "A garden gate in the northwest corner leads to the court garden.\n\n"
        "Races of all kinds mingle here without hostility. A Cyber-Knight\n"
        "shares a bench with an Elven mage. Children of three different\n"
        "species play near the garden gate. This is what New Camelot aspires to.");
    set_exits( ([
        "south"     : "/domains/NewCamelot/areas/newcamelot_start",
        "north"     : "/domains/NewCamelot/areas/camelot_tavern",
        "east"      : "/domains/NewCamelot/areas/camelot_armoury",
        "west"      : "/domains/NewCamelot/areas/camelot_wizard_tower",
        "northwest" : "/domains/NewCamelot/areas/camelot_garden",
        "northeast" : "/domains/NewCamelot/areas/market_street1"
    ]) );
    set_listen("default",
        "Market noise, laughter, the splash of the fountain, and the distant "
        "sound of a bard warming up.");
    set_smell("default",
        "Fresh bread, magical energy, horse, and the clean smell of the fountain.");
    set_items( ([
        "round table" : "A replica of the legendary Round Table. All seats equal.",
        "table" : "A replica of the legendary Round Table. All seats equal.",
        "fountain" : "The water shimmers with faint magical energy. Touching it is oddly calming.",
        "stalls" : "Market stalls from across Europe. Good variety.",
        "monument" : "The Round Table monument at the center of the plaza."
    ]) );
}

void reset() {
    ::reset();
    if(!present("moxim", this_object()))
        clone_object(MOXIM_PATH)->move(this_object());
}
