// /domains/newcamelot/areas/camelot_wizard_tower.c
// Tower of the Camelot Order. Scroll vendor. 100cr entry donation.

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/newcamelot/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Tower of the Camelot Order, Entry Hall");
    set_long(
        "A tall stone tower with a faint magical glow visible at the top.\n"
        "The entry hall is lined with bookshelves and magical diagrams on\n"
        "parchment. Crystals in wall sconces provide steady light. The air\n"
        "hums with contained magical energy.\n\n"
        "Entry requires a donation of 100 credits to the Order.");
    set_exits( ([
        "east" : "/domains/newcamelot/areas/camelot_square",
        "west" : "/domains/newcamelot/areas/academy_quad"
    ]) );
    set_listen("default",
        "The quiet crackle of contained magical energy and the turning of pages.");
    set_smell("default",
        "Old books, candle wax, and the sharp clean smell of focused magical energy.");
    set_items( ([
        "bookshelves" : "Floor-to-ceiling shelves packed with texts in multiple scripts.",
        "diagrams" : "Parchment sheets pinned to the walls: spell circles and ley line maps.",
        "crystals" : "Light crystals in the wall sconces. They don't flicker.",
        "desk" : "The mage's reading desk. Covered in open texts.",
        "tomes" : "Ancient tomes. Most are in scripts you don't recognize."
    ]) );
}

void reset() {
    ::reset();
    if(!present("camelot mage", this_object()))
        clone_object(NPC_PATH+"camelot_mage")->move(this_object());
}
