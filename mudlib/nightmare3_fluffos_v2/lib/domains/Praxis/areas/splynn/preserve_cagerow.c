// /d/Praxis/areas/splynn/preserve_cagerow.c
// Splynn Preserves hunting loop, R17: Cage Row, the slavers' holding pen.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Cage Row");
    set_long(
        "South of the hollow the ground has been dug out and roofed over\n"
        "into a row of alien-mesh cages, sunk half below grade so the\n"
        "light barely reaches the bars. This is where the Splugorth hold\n"
        "prey worth more alive than dead, stock and captured D-bees held\n"
        "back from the field until they are wanted. Most of the cages\n"
        "stand rusted open and empty, waiting for the next drive.\n\n"
        "The way out is back north, up into the hollow. A ladder of\n"
        "corroded rungs also drops south, into a second pit dug even\n"
        "deeper below grade.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/preserve_hollow",
        "south" : "/domains/Praxis/areas/splynn/preserve_cagepit"
    ]) );
    set_listen("default",
        "The creak of alien mesh settling, and, from one of the cages, the\n"
        "small sound of something trying very hard not to be heard.");
    set_smell("default",
        "Rust, damp earth, and the sour animal smell of cages that have\n"
        "held prey too long between drives.");
    set_items( ([
        "cages" : "A row of sunken cages roofed in alien mesh, most rusted\n"
                  "open and empty.",
        "mesh"  : "Woven alien mesh, corroded but still strong enough to\n"
                  "hold what it was built to hold.",
        "bars"  : "Bars sunk into the dug-out floor, close enough together\n"
                  "that nothing bigger than a hand fits through."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Splugorth minion", this_object()))
        clone_object(MONSTER_PATH+"splugorth_minion.c")->move(this_object());
    if(!present("captive", this_object()))
        clone_object(MONSTER_PATH+"preserve_captive.c")->move(this_object());
}
