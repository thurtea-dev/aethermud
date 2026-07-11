// /domains/tolkeen/areas/tolkeen_no_mans_land.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, No Man's Land");
    set_long(
        "Open ground between trench and wall. Craters. Wire. Ward burns\n"
        "scar the dirt in perfect circles. Crossing here draws fire from\n"
        "someone - always.\n\n"
        "The CS trench is south. Siege edge under the east wall is north.");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_cs_trench",
        "north" : "/domains/tolkeen/areas/tolkeen_siege_edge"
    ]) );
    set_listen("default", "Your own footsteps. Too loud.");
    set_smell("default", "Ozone and dust.");
}

void reset() {
    ::reset();
    if(!present("cs siege scout", this_object()) && !random(3))
        clone_object("/domains/tolkeen/monsters/cs_siege_scout")->move(this_object());
}
