// /d/Praxis/areas/splynn/preserve_blackwater.c
// Splynn Preserves hunting loop, R24: Blackwater, the deep end of the
// Sunken Shallows and the thing that has been living in it.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Blackwater");
    set_long(
        "The silt bottom drops away completely here. Light does not reach\n"
        "past the first few feet; past that the water is simply black,\n"
        "cold, and old. Whatever moves in the shallows to the west was\n"
        "only ever passing through. This is where it lives.\n\n"
        "There is no shore, no footing, nothing to stand on. Swimming\n"
        "further out means leaving the last solid ground behind.\n\n"
        "The shallows lie back west.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/preserve_sunken"
    ]) );
    set_listen("default",
        "Nothing. No lapping, no wind, no birds. Water this still should\n"
        "not be this quiet.");
    set_smell("default",
        "Cold mineral water with no rot in it at all, which is somehow\n"
        "worse than if there were.");
    set_items( ([
        "water"  : "Black, bottomless-looking water. The last light gives\n"
                   "out a few feet down.",
        "depths" : "Whatever lives down here has had a long time to grow\n"
                   "into the space undisturbed.",
        "silt"   : "The bottom vanishes into silt and dark well before the\n"
                   "shallows do. This is deep water pretending to be a\n"
                   "pond."
    ]) );
}

void reset() {
    ::reset();
    if(!present("sea serpent", this_object()))
        clone_object(MONSTER_PATH+"sea_serpent.c")->move(this_object());
}
