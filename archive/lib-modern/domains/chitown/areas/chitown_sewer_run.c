// /domains/chitown/areas/chitown_sewer_run.c
// Narrow sewer tunnel beneath the Chi-Town burbs.

#include <std.h>
#include <rooms.h>

#define MOB_PATH "/domains/chitown/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_short("Chi-Town Sewer Run");
    set_long(
        "A cramped concrete tunnel barely wide enough for two people to pass.\n"
        "Slick black water runs along a channel cut into the floor. Pipes and\n"
        "cables hang overhead, dripping condensation into the murk. Graffiti\n"
        "marks the walls in Coalition stencil and Burbie scrawl.\n\n"
        "The stair up to the burbs grate is above. The tunnel continues east\n"
        "toward an old overflow chamber.");
    set_exits( ([
        "up"   : "/domains/chitown/areas/chitown_sewer_entrance",
        "east" : "/domains/chitown/areas/chitown_sewer_overflow"
    ]) );
    set_listen("default",
        "Water trickling through pipes. Something skitters in the dark.");
    set_smell("default",
        "Stagnant water, mildew, and chemical runoff from the city above.");
    set_items( ([
        "water" : "Black runoff moving slowly toward the east.",
        "pipes" : "Old service pipes patched with welded plate.",
        "graffiti" : "CS patrol counts, gang tags, and one faded 'FREEDOM CITY' slogan."
    ]) );
}

void reset() {
    ::reset();
    if(!present("tunnel rat", this_object()))
        clone_object(MOB_PATH + "chitown_tunnel_rat")->move(this_object());
}
