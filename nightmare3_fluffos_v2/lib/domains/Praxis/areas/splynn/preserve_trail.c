// /d/Praxis/areas/splynn/preserve_trail.c
// Splynn Preserves hunting loop, R1: the game trail leading in from the core.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Game Trail");
    set_long(
        "A beaten track worn into the alien soil, wide enough that whatever\n"
        "made it was large and used it often. Clawed prints press deep into\n"
        "the mud in both directions, some of them fresh. The undergrowth\n"
        "leans in close on either side, purple and waxy, so that the trail\n"
        "feels like a corridor with a living roof.\n\n"
        "This is a predator road. Prey does not walk it by choice. The\n"
        "Preserves core lies back to the east, the trail runs west into\n"
        "deeper hunting ground, and a narrower track forks north where the\n"
        "undergrowth has been trampled thin.");
    set_exits( ([
        "east"  : "/domains/Praxis/areas/splynn/splynn_preserves",
        "west"  : "/domains/Praxis/areas/splynn/preserve_thornbrake",
        "north" : "/domains/Praxis/areas/splynn/preserve_switchback"
    ]) );
    set_listen("default",
        "The soft give of mud under something heavy, somewhere just out of\n"
        "sight, keeping pace with you.");
    set_smell("default",
        "Churned soil, crushed vegetation, and the rank musk of a big\n"
        "animal that came through not long ago.");
    set_items( ([
        "prints" : "Deep clawed tracks in the mud. Four-toed, longer than your\n"
                   "hand, and pressed in hard. Whatever left them has weight.",
        "trail"  : "A wide beaten road through the undergrowth, packed flat by\n"
                   "years of heavy traffic. Predators made this, not people.",
        "undergrowth" : "Waxy purple growth pressing in on both sides. Perfect\n"
                        "cover for something waiting beside the trail."
    ]) );
}

void reset() {
    ::reset();
    if(!present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
}
