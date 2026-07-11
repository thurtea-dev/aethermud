// /domains/tolkeen/areas/tolkeen_cs_trench.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, CS Forward Trench");
    set_long(
        "A muddy trench cut toward Tolkeen's east wall. Empty ration packs\n"
        "with CS stamps. Sandbags. Someone left in a hurry - or did not leave.\n\n"
        "Ruined suburbs are west. No-man's-land stretches north toward the\n"
        "wall.");
    set_exits( ([
        "west"  : "/domains/tolkeen/areas/tolkeen_ruined_suburb",
        "north" : "/domains/tolkeen/areas/tolkeen_no_mans_land"
    ]) );
    set_listen("default", "Flies. A loose tarp flapping.");
    set_smell("default", "Mud and old blood.");
}

void reset() {
    ::reset();
    if(!present("cs siege scout", this_object()))
        clone_object("/domains/tolkeen/monsters/cs_siege_scout")->move(this_object());
}
