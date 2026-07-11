// /domains/tolkeen/areas/tolkeen_tower_ne.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Northeast Tower");
    set_long(
        "A round tower room packed with signal crystals and a heavy ballista\n"
        "aimed east. Slits look out over the siege edge.\n\n"
        "North wall walk west. East wall walk south.");
    set_exits( ([
        "west"  : "/domains/tolkeen/areas/tolkeen_wall_n",
        "south" : "/domains/tolkeen/areas/tolkeen_wall_e"
    ]) );
    set_listen("default", "Crystal hum and wind through arrow slits.");
    set_smell("default", "Cold stone.");
}

void reset() {
    ::reset();
    if(!present("tolkeen militia", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_militia")->move(this_object());
}
