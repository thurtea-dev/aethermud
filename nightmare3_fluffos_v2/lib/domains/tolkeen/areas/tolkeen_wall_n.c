// /domains/tolkeen/areas/tolkeen_wall_n.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, North Wall Walk");
    set_long(
        "Northern battlements. Ward pylons line the parapet. Through the\n"
        "haze, ley storms roll across wilds beyond Tolkeen's claim.\n\n"
        "Liberation Square is south. Northeast tower east. Northwest tower\n"
        "west.");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_square",
        "east"  : "/domains/tolkeen/areas/tolkeen_tower_ne",
        "west"  : "/domains/tolkeen/areas/tolkeen_tower_nw"
    ]) );
    set_listen("default", "Wind and a ward pylon's low tone.");
    set_smell("default", "Cold air and ozone.");
}

void reset() {
    ::reset();
    if(!present("tolkeen militia", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_militia")->move(this_object());
}
