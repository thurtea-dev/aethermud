// /domains/LoneStar/areas/lone_star_containment.c
// Lone Star containment wing for failed genetic experiments.

#define NPC_PATH "/domains/LoneStar/npcs/"
#define MON_PATH "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Containment Wing");
    set_long(
        "A corridor of reinforced cells with observation windows. Most doors\n"
        "are sealed, but one cell stands open. Warning strobes are dark for\n"
        "now, though scratch marks on the inner walls suggest recent trouble.\n\n"
        "Coalition scientists call this wing 'temporary holding.' The subjects\n"
        "inside rarely leave alive. Some escape anyway.");
    set_exits( ([
        "south" : "/domains/LoneStar/areas/lone_star_lab",
        "east"  : "/domains/LoneStar/areas/lone_star_perimeter"
    ]) );
    set_listen("default",
        "A distant mechanical hum. Something taps against glass, then stops.");
    set_smell("default",
        "Antiseptic and something organic that does not belong in a clean lab.");
    set_items( ([
        "cells" : "Reinforced holding cells with one-way glass. Most are occupied.",
        "scratches" : "Deep gouges on the inside of one cell door, as if claws tried to escape.",
        "window" : "Observation glass streaked with something you do not want to identify.",
        "strobes" : "Emergency lights, currently off."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Dog Boy guard", this_object()))
        clone_object(NPC_PATH + "cs_dog_boy_guard.c")->move(this_object());
    if(!present("mutant subject", this_object()) && !random(2))
        clone_object(MON_PATH + "ls_mutant_subject.c")->move(this_object());
}
