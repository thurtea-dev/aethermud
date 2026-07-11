// /domains/tolkeen/areas/tolkeen_wall_w.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, West Wall Walk");
    set_long(
        "Western battlements overlook refugee cookfires inside and dark\n"
        "fields outside. A ley compass on a pedestal spins until someone\n"
        "calibrates it.\n\n"
        "Barracks are east. Northwest tower is north.");
    set_exits( ([
        "east"  : "/domains/tolkeen/areas/tolkeen_barracks",
        "north" : "/domains/tolkeen/areas/tolkeen_tower_nw"
    ]) );
    set_listen("default", "Wind. A child crying far below inside the wall.");
    set_smell("default", "Cooksmoke from the refugee yard.");
}
