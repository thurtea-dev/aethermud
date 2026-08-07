// /d/Praxis/areas/splynn/splynn_preserves.c
// The Splynn Preserves: hunting grounds maintained by the Splugorth.
// Exotic creatures from multiple dimensions are kept here and hunted for sport.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("The Splynn Preserves");
    set_long(
        "Beyond the southern edge of the city, the Splugorth maintain a\n"
        "vast tract of enclosed wilderness: the Preserves. The outer wall\n"
        "is energy-barrier technology, invisible but absolute. Inside,\n"
        "a managed ecosystem of creatures from multiple dimensions has been\n"
        "cultivated, maintained, and hunted for Splugorth sport.\n\n"
        "The terrain is dense: alien plant life in impossible colors, thick\n"
        "undergrowth, and the occasional bone-white tree that grows in\n"
        "geometric spirals. Everything here is either prey or predator.\n"
        "You do not know which category you fall into yet.\n\n"
        "Movement in the undergrowth suggests you are not alone.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/splynn_south_road",
        "east"  : "/domains/Praxis/areas/splynn/splynn_forest_edge",
        "west"  : "/domains/Praxis/areas/splynn/preserve_trail"
    ]) );
    set_listen("default",
        "Something large moving through undergrowth to the east. Birds,\n"
        "or things that sound like birds, calling in patterns that do not\n"
        "match any species from Earth.");
    set_smell("default",
        "Dense vegetation, something musky and predatory, and the faint\n"
        "ozone hum of the containment barrier somewhere behind you.");
    set_items( ([
        "undergrowth" : "Dense alien plant life: thick, waxy leaves in deep purple\n"
                        "and structural greens. Good cover. Also good ambush terrain.",
        "trees" : "Bone-white trees growing in geometric spirals. Pre-Rifts\n"
                        "Earth flora cannot survive here. This all came through rifts.",
        "barrier" : "The energy barrier is invisible from inside. You will know\n"
                        "you have found it when you cannot go further.",
        "movement" : "Something in the undergrowth has been tracking you since you\n"
                        "entered. You have not seen it clearly yet.",
        "wall" : "The outer containment wall. MDC-rated and monitored.\n"
                        "Creatures from outside cannot get in. Creatures from inside cannot get out."
    ]) );
}

void reset() {
    ::reset();
    if(!present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
    if(random(10) < 6 && !present("xiticix warrior", this_object()))
        clone_object(MONSTER_PATH+"xiticix_warrior.c")->move(this_object());
    if(random(10) < 4 && !present("splugorth minion", this_object()))
        clone_object(MONSTER_PATH+"splugorth_minion.c")->move(this_object());
    if(random(100) < 15 && !present("kydian overlord", this_object()))
        clone_object(MONSTER_PATH+"kydian_overlord.c")->move(this_object());
}
