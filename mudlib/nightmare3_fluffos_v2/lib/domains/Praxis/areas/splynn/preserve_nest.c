// /d/Praxis/areas/splynn/preserve_nest.c
// Splynn Preserves hunting loop, R19: High Nest, a dead end off the switchback.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("High Nest");
    set_long(
        "The switchback ends at a wide ledge where the rock shoulders out\n"
        "into a natural bowl, floored with broken branches, matted fur,\n"
        "and the picked-clean bones of things dragged up from below. This\n"
        "is a nest, built by something that hunts the switchback trail as\n"
        "its private larder.\n\n"
        "The way down is back south, along the switchback. A narrow spine\n"
        "of exposed rock also leads west, along the ridgeline.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/preserve_switchback",
        "west"  : "/domains/Praxis/areas/splynn/preserve_saddle"
    ]) );
    set_listen("default",
        "Wind scouring bare rock, and the heavy, unhurried tread of\n"
        "something settling back into the nest.");
    set_smell("default",
        "Sun-bleached bone, old fur, and a sharp animal musk baked into\n"
        "the rock.");
    set_items( ([
        "nest"  : "A natural rock bowl floored with broken branches and\n"
                  "matted fur, built up over a long time.",
        "bones" : "Picked-clean bones, dragged up the switchback from lower\n"
                  "ground and left to bleach in the sun.",
        "fur"   : "Matted fur pressed into the nest floor, shed or torn\n"
                  "from something large."
    ]) );
}

void reset() {
    ::reset();
    if(!present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
}
