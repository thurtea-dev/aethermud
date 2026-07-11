// /domains/tolkeen/areas/tolkeen_barracks.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Militia Barracks");
    set_long(
        "Cots and weapon racks fill a reinforced hall. Maps of Coalition\n"
        "advance lines cover one wall, updated in chalk daily. A door leads\n"
        "to the west wall walk for quick deployment.\n\n"
        "Liberation Square is east. West wall access is west. An armory\n"
        "closet is north.");
    set_exits( ([
        "east"  : "/domains/tolkeen/areas/tolkeen_square",
        "west"  : "/domains/tolkeen/areas/tolkeen_wall_w",
        "north" : "/domains/tolkeen/areas/tolkeen_armory"
    ]) );
    set_listen("default", "Boot polish, locker latches, quiet talk.");
    set_smell("default", "Oil and canvas.");
}

void reset() {
    ::reset();
    if(!present("tolkeen militia", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_militia")->move(this_object());
}
