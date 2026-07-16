// /d/Praxis/areas/splynn/preserve_catwalk.c
// Splynn Preserves hunting loop, R14: High Catwalk, a second blind off the first.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("High Catwalk");
    set_long(
        "A narrow plank walkway runs north from the first blind, strung\n"
        "between the bone-white trees at a dizzying height, ending at a\n"
        "second, smaller firing post lashed into the canopy. The Splugorth\n"
        "built this ground for hunters who wanted the higher vantage and\n"
        "did not mind the drop.\n\n"
        "From this high the Preserves spread out in every direction, alien\n"
        "green and purple broken by the pale scar of the bone field. The\n"
        "only way back down is south along the catwalk.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/preserve_blind"
    ]) );
    set_listen("default",
        "Wind moving through the high canopy, the creak of rope lashings\n"
        "under your weight, and something below moving unseen through the\n"
        "brush.");
    set_smell("default",
        "Clean high air, tar-sealed rope, and, faint from below, the same\n"
        "cold reptilian scent that hangs around the lower blind.");
    set_items( ([
        "catwalk"  : "A plank walkway strung between the trees at height,\n"
                     "lashed with tarred rope. It sways underfoot.",
        "post"     : "A small firing post at the catwalk's end, barely large\n"
                     "enough for one shooter and a spotter.",
        "rope"     : "Thick, tar-sealed rope lashings holding the whole\n"
                     "structure to the trees. Old, but well maintained."
    ]) );
}

void reset() {
    ::reset();
    if(random(10) < 5 && !present("splugorth minion", this_object()))
        clone_object(MONSTER_PATH+"splugorth_minion.c")->move(this_object());
}
