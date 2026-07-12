// /d/Praxis/areas/splynn/preserve_blind.c
// Splynn Preserves hunting loop, R5: the Splugorth hunting blind.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Slaver's Blind");
    set_long(
        "A platform of dark alien timber stands raised above the field on\n"
        "four legs, roofed and screened, with slots cut to fire through. This\n"
        "is a hunting blind, built so that a Splugorth slaver and its guards\n"
        "can watch the bone field below and pick off whatever is driven into\n"
        "the open. Spent power cells and the stubs of exotic ammunition\n"
        "litter the floorboards.\n\n"
        "From up here the whole clearing spreads out west. The platform\n"
        "connects east along a walkway toward a chittering outcrop of hive\n"
        "matter.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/preserve_bonefield",
        "east" : "/domains/Praxis/areas/splynn/preserve_hive"
    ]) );
    set_listen("default",
        "Boards creaking under a weight that is not yours, and the low\n"
        "clicking speech of something that hunts for pleasure.");
    set_smell("default",
        "Scorched propellant, oiled metal, and the cold reptilian scent of\n"
        "the thing that keeps this blind.");
    set_items( ([
        "platform" : "A raised firing platform of dark timber, screened and\n"
                     "roofed, angled to overlook the bone field.",
        "slots"    : "Firing slots cut through the screen, each with a clear\n"
                     "line down onto the open ground below.",
        "cells"    : "Spent power cells and exotic ammunition stubs kicked into\n"
                     "the corners. The hunters here are well armed and wasteful."
    ]) );
}

void reset() {
    ::reset();
    if(!present("splugorth minion", this_object()))
        clone_object(MONSTER_PATH+"splugorth_minion.c")->move(this_object());
}
