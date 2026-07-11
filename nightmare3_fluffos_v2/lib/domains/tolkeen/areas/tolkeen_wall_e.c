// /domains/tolkeen/areas/tolkeen_wall_e.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, East Wall Walk");
    set_long(
        "Eastern battlements face the siege lines. Scorch marks blacken the\n"
        "outer stone. Beyond the wall, trenches and ruined suburbs mark how\n"
        "close the Coalition has come.\n\n"
        "Northeast tower is north. A postern gate east drops to the siege\n"
        "approach - dangerous.");
    set_exits( ([
        "north" : "/domains/tolkeen/areas/tolkeen_tower_ne",
        "east"  : "/domains/tolkeen/areas/tolkeen_siege_edge"
    ]) );
    set_listen("default", "Distant rifle cracks. Ward alarms cycling.");
    set_smell("default", "Smoke on the wind.");
}
