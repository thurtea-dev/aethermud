// /domains/newcamelot/areas/camelot_tavern.c
// The Pendragon Inn. Warmth, drinks, and rumors.

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
    set_short("The Pendragon Inn");
    set_long(
        "Warm firelight fills the Pendragon Inn. Heavy oak beams cross the\n"
        "ceiling. A bard in the corner plays something slow and melancholic\n"
        "on a lute. The fire crackles in a great stone hearth. The smell of\n"
        "roasted food and spilled ale fills the air.\n\n"
        "The bar itself is long hardwood, polished smooth by years of use.");
    set_exits( ([ "south" : "/domains/newcamelot/areas/camelot_square" ]) );
    set_listen("default",
        "The bard's lute, the fire, low conversation, and the clunk of tankards.");
    set_smell("default",
        "Roasted meat, spilled ale, woodsmoke, and candle wax.");
    set_items( ([
        "bar" : "Long hardwood bar. The surface is polished from years of use.",
        "hearth" : "Great stone fireplace. The fire is well-maintained.",
        "bard" : "A traveling musician, playing something old and wistful.",
        "tables" : "Heavy oak tables with benches. Several occupied by locals.",
        "tankards" : "Battered pewter tankards. Well-used."
    ]) );
}

void reset() {
    ::reset();
    if(!present("pendragon barkeep", this_object()))
        clone_object(NPC_PATH+"pendragon_barkeep")->move(this_object());
}
