// /domains/Praxis/areas/splynn/alvurron/alvurron_market.c
// Gargoyle trade row north of the Alvurron plaza.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, Trade Row");
    set_long(
        "A row of black stone stalls run by gargoyles who deal in salvage,\n"
        "hides, and dimensional curiosities pulled from the rifts beneath the\n"
        "city. Witch-lamps gutter over crooked awnings. The traders watch\n"
        "newcomers with sharp interest but do not approach uninvited. Credits\n"
        "are accepted. Questions are not. The plaza is south.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/alvurron/alvurron_square"
    ]) );
    set_listen("default",
        "The clack of claws on stone and low haggling in a language that is\n"
        "mostly growls.");
    set_smell("default", "Hide, scorched stone, and old ozone.");
    set_items( ([
        "stalls"   : "Crooked stone stalls hung with hides and salvage.\n"
                      "Nothing here has a price tag. You ask, they name a figure.",
        "traders"  : "Gargoyle traders, sharp-eyed and unhurried. They have seen\n"
                      "every kind of buyer there is.",
        "curiosities" : "Dimensional oddities pulled from the rifts below the city.\n"
                      "Most of it you cannot identify."
    ]) );
}

void reset() {
    ::reset();
    if(!present("gargoyle trader", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_trader")->move(this_object());
}
