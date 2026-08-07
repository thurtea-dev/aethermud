// /d/Praxis/areas/splynn/preserve_cagepit.c
// Splynn Preserves hunting loop, R23: Lower Cage Pit, a deeper holding
// pit below Cage Row.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lower Cage Pit");
    set_long(
        "A ladder of corroded rungs drops from Cage Row into a second,\n"
        "deeper pit dug even further below grade. The cages down here are\n"
        "older, cruder, and packed closer together. This is where the\n"
        "Splugorth keep stock they are not yet ready to bring up, or have\n"
        "simply forgotten about.\n\n"
        "The rungs lead back up to Cage Row. A gap in the pit's far wall,\n"
        "half-collapsed, opens onto something older still.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/preserve_cagerow",
        "down"  : "/domains/Praxis/areas/splynn/preserve_oldhold"
    ]) );
    set_listen("default",
        "Water dripping somewhere unseen, and low, defeated sounds from\n"
        "more than one of the cages.");
    set_smell("default",
        "Wet earth and the same sour animal reek as Cage Row above, but\n"
        "thicker, older, unaired.");
    set_items( ([
        "cages" : "Older, cruder cages than the ones above, packed closer\n"
                  "together with barely room to stand between them.",
        "rungs" : "A ladder of corroded rungs leading back up to Cage Row.",
        "pit"   : "A second, deeper pit dug below Cage Row, dark and\n"
                  "close."
    ]) );
}

void reset() {
    ::reset();
    if(!present("captive", this_object()))
        clone_object(MONSTER_PATH+"preserve_captive.c")->move(this_object());
    if(!present("Splugorth minion", this_object()))
        clone_object(MONSTER_PATH+"splugorth_minion.c")->move(this_object());
}
