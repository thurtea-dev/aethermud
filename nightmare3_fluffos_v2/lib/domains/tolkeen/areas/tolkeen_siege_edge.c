// /domains/tolkeen/areas/tolkeen_siege_edge.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, East Siege Edge");
    set_long(
        "Hard against the outer east wall. Scorch marks climb the stone.\n"
        "A postern gate above leads back onto the wall walk - if the militia\n"
        "still trust you.\n\n"
        "No-man's-land is south. The east wall walk is west (up the postern).");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_no_mans_land",
        "west"  : "/domains/tolkeen/areas/tolkeen_wall_e"
    ]) );
    set_listen("default", "Wall ward hum overhead.");
    set_smell("default", "Burned stone.");
}
