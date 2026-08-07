// /d/Praxis/areas/splynn/preserve_snareline.c
// Splynn Preserves hunting loop, R9: the old snare line, rejoins forest edge.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Old Snare Line");
    set_long(
        "A line of Splugorth traps runs across the ground here, strung where\n"
        "the hunting preserve gives way to true forest. Most have long since\n"
        "sprung: rusted jaws clamped shut on nothing, snare wires slack in\n"
        "the grass, a pit or two collapsed and grown over. A few might still\n"
        "have a bite left in them for the careless.\n\n"
        "This is the edge of the killing ground. The ridge climbs back west\n"
        "into the Preserves proper, and north the alien undergrowth thins\n"
        "into the clearing at the forest's edge.");
    set_exits( ([
        "west"  : "/domains/Praxis/areas/splynn/preserve_ridge",
        "north" : "/domains/Praxis/areas/splynn/splynn_forest_edge"
    ]) );
    set_listen("default",
        "The creak of old wire under tension, and the small sounds of\n"
        "something that learned long ago to step around the traps.");
    set_smell("default",
        "Rust, wet rope, and the green wet smell of real forest drifting in\n"
        "from the north to replace the preserve's rot.");
    set_items( ([
        "traps"  : "A line of Splugorth hunting traps: jaw-traps, snares, and\n"
                   "grown-over pits. Most are sprung. A few are not.",
        "jaws"   : "Rusted iron trap-jaws clamped shut on empty air, strong\n"
                   "enough still to take a foot off at the ankle.",
        "wires"  : "Snare wires laid slack in the grass, waiting for a leg.\n"
                   "Old, but not all of them are safely dead."
    ]) );
}

void reset() {
    ::reset();
    if(random(10) < 4 && !present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
}
