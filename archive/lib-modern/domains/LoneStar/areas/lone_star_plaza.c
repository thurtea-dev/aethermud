// /domains/LoneStar/areas/lone_star_plaza.c
// Lone Star complex interior plaza -- open area between the main buildings.

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 0);
    set_short("Lone Star: Complex Plaza");
    set_long(
        "The interior plaza of Lone Star is a wide paved square between the\n"
        "research buildings and the barracks block.  Prefab Coalition buildings\n"
        "in uniform gray rise on three sides.  A central flagpole flies the\n"
        "Coalition banner above a concrete plinth with an inscription.\n\n"
        "Research staff in white lab coats move briskly between buildings.  Dog\n"
        "Boy soldiers patrol in pairs along the perimeter.  The air smells of\n"
        "antiseptic and ozone.");
    set_exits( ([
        "west"  : "/domains/LoneStar/areas/lone_star_gate",
        "north" : "/domains/LoneStar/areas/lone_star_lab",
        "east"  : "/domains/LoneStar/areas/lone_star_barracks",
        "south" : "/domains/LoneStar/areas/lone_star_supply"
    ]) );
    set_listen("default",
        "Boots on pavement.  A distant bark from deeper in the facility.  The flutter of the Coalition banner.");
    set_smell("default",
        "Antiseptic cleaning solution overlaid with the faint chemical tang of genetic lab work.");
    set_items( ([
        "flagpole" : "A standard Coalition flagpole, the skull-on-black banner flying overhead.",
        "banner"   : "The Coalition skull banner, black on white.  Emperor Prosek's symbol.",
        "plinth"   : "A concrete base with a bronze plaque.  It reads: LONE STAR COMPLEX, ESTABLISHED YEAR 64 P.A.",
        "plaque"   : "Bronze.  LONE STAR COMPLEX, ESTABLISHED YEAR 64 P.A.",
        "buildings": "Identical prefab Coalition gray, labeled by number rather than name.",
        "staff"    : "Scientists and technicians moving between buildings with purpose."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Dog Boy guard", this_object()))
        clone_object(NPC_PATH + "cs_dog_boy_guard.c")->move(this_object());
    if(!present("Coalition scientist", this_object()))
        clone_object(NPC_PATH + "coalition_scientist.c")->move(this_object());
}
