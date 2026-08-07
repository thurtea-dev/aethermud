// /d/Praxis/areas/splynn/preserve_sunken.c
// Splynn Preserves hunting loop, R18: Sunken Shallows, a dead end off the reeds.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Sunken Shallows");
    set_long(
        "East of the reeds the ground gives way entirely, the water hole\n"
        "deepening into open water thick with silt and drifting alien\n"
        "weed. The bottom drops away fast a few steps out, and something\n"
        "moves beneath the surface that is bigger than anything that\n"
        "should be living in a hole this size.\n\n"
        "The reeds and the rest of the hunting ground lie back west. The\n"
        "water only gets darker and deeper further east.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/preserve_reeds",
        "east" : "/domains/Praxis/areas/splynn/preserve_blackwater"
    ]) );
    set_listen("default",
        "Water lapping against something unseen just below the surface,\n"
        "and a slow displacement wave that reaches the bank a moment\n"
        "later.");
    set_smell("default",
        "Stagnant water, rotting weed, and a cold mineral scent that has\n"
        "no business coming from standing water this shallow.");
    set_items( ([
        "water"   : "Dark, silt-thick water, deep enough to hide whatever\n"
                    "is moving under the surface.",
        "weed"    : "Drifting alien weed, thick ropes of it curling just\n"
                    "under the waterline.",
        "ripples" : "Slow ripples crossing the surface with no wind to\n"
                    "explain them."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ocean creature", this_object()))
        clone_object(MONSTER_PATH+"ocean_creature.c")->move(this_object());
}
