// /d/Praxis/areas/splynn/preserve_ridge.c
// Splynn Preserves hunting loop, R8: the overgrown ridge, a high vantage.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Overgrown Ridge");
    set_long(
        "The ground heaves up into a low ridge, its crest matted with wiry\n"
        "alien grass that clutches at your legs. From the top the Preserves\n"
        "spread out below in every direction, a sea of purple undergrowth\n"
        "broken by the pale scar of the bone field, and far off to the east\n"
        "the black spires of Splynn itself catch the light above the\n"
        "containment haze.\n\n"
        "It is the only place in the hunting ground where you can see where\n"
        "you are. The snag lies back down the slope to the south; the ridge\n"
        "runs east toward the forest's ragged edge.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/preserve_snag",
        "east"  : "/domains/Praxis/areas/splynn/preserve_snareline"
    ]) );
    set_listen("default",
        "Wind moving through the ridge grass, and carried on it the faint\n"
        "layered noise of the whole preserve hunting itself far below.");
    set_smell("default",
        "Clean moving air for once, dry grass, and only the faintest thread\n"
        "of the rot that fills the low ground.");
    set_items( ([
        "spires" : "The black towers of Splynn, far east across the Preserves,\n"
                   "shimmering above the faint haze of the containment barrier.",
        "grass"  : "Wiry alien grass matting the ridge crest, tough enough to\n"
                   "snag your legs and hide what waits low in it.",
        "field"  : "The pale scar of the bone field, visible from up here as a\n"
                   "bare wound in the purple sea of undergrowth below."
    ]) );
}

void reset() {
    ::reset();
    if(random(10) < 5 && !present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
}
