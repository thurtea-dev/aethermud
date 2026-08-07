// /d/Praxis/areas/splynn/preserve_oldhold.c
// Splynn Preserves hunting loop, R25: Old Hold, the stock the Splugorth
// forgot, below the Lower Cage Pit.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Old Hold");
    set_long(
        "The gap in the pit wall opens into a hold older than anything\n"
        "above it: hand-dug, unreinforced, the kind of holding cell that\n"
        "predates whatever system Cage Row runs on now. The cages here are\n"
        "wood and rusted iron, not alloy. Some are broken open from the\n"
        "inside. Some are not.\n\n"
        "Nobody has restocked this hold in a long time. Nobody has emptied\n"
        "it either.\n\n"
        "The gap back up to the Lower Cage Pit is the only way out.");
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/preserve_cagepit"
    ]) );
    set_listen("default",
        "Nothing moves quickly down here. Something moves, eventually,\n"
        "slowly, dragging.");
    set_smell("default",
        "Old iron rust and a closed-in staleness with no fresh air behind\n"
        "it at all.");
    set_items( ([
        "cages"  : "Wood and rusted iron, older construction than anything\n"
                   "in Cage Row above. Some stand open. Some do not.",
        "hold"   : "A hand-dug holding cell that predates the current Cage\n"
                   "Row system entirely. Forgotten stock, forgotten by\n"
                   "whoever forgets things like this.",
        "gap"    : "The half-collapsed gap in the Lower Cage Pit's wall,\n"
                   "leading back up."
    ]) );
}

void reset() {
    ::reset();
    if(!present("captive", this_object()))
        clone_object(MONSTER_PATH+"preserve_captive.c")->move(this_object());
    if(!present("Splugorth minion", this_object()))
        clone_object(MONSTER_PATH+"splugorth_minion.c")->move(this_object());
}
