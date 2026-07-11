// /domains/LoneStar/areas/lone_star_gate.c
// Lone Star complex outer gate -- checkpoint on the eastern road.

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_property("vehicle_accessible", 1);
    set_short("Lone Star: Outer Gate");
    set_long(
        "A reinforced Coalition checkpoint spans the road.  Two guard towers\n"
        "rise on either side, each manned by Dog Boys with clear sightlines\n"
        "down the approach.  The gate itself is a thick alloy barrier bearing\n"
        "the Coalition skull insignia painted in faded white.\n\n"
        "A sign overhead reads: LONE STAR GENETICS RESEARCH COMPLEX / CLASS IV\n"
        "RESTRICTED / AUTHORIZED PERSONNEL ONLY / VIOLATORS WILL BE DETAINED.");
    set_exits( ([
        "west"  : "/domains/Praxis/areas/nexus_approach",
        "east"  : "/domains/LoneStar/areas/lone_star_plaza",
        "north" : "/domains/LoneStar/areas/lone_star_perimeter"
    ]) );
    set_listen("default",
        "Wind across flat ground.  The low hum of the gate's power supply.");
    set_smell("default",
        "Dust and the faint chemical smell of industrial coating on the alloy gate.");
    set_items( ([
        "gate"     : "A thick alloy barrier, Coalition-issue.  The skull logo is painted across it in faded white.",
        "towers"   : "Two guard towers with clear sightlines down the road.  Dog Boys at the top.",
        "sign"     : "LONE STAR GENETICS RESEARCH COMPLEX / CLASS IV RESTRICTED / AUTHORIZED PERSONNEL ONLY / VIOLATORS WILL BE DETAINED.",
        "skull"    : "The Coalition skull insignia, painted large on the gate barrier.",
        "checkpoint" : "A standard Coalition military checkpoint.  Well-maintained."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Dog Boy guard", this_object()))
        clone_object(NPC_PATH + "cs_dog_boy_guard.c")->move(this_object());
}
