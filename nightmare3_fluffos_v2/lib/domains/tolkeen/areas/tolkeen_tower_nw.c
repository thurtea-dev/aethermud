// /domains/tolkeen/areas/tolkeen_tower_nw.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Northwest Tower");
    set_long(
        "A quieter tower. Maps of ley weather cover a table. A kettle sits\n"
        "on a ward-heated stone.\n\n"
        "North wall walk east. West wall walk south.");
    set_exits( ([
        "east"  : "/domains/tolkeen/areas/tolkeen_wall_n",
        "south" : "/domains/tolkeen/areas/tolkeen_wall_w"
    ]) );
    set_listen("default", "Soft bubbling from the kettle.");
    set_smell("default", "Tea and ozone.");
}
