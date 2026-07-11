// /domains/tolkeen/areas/tolkeen_wall_s.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_property("domain_stub", 1);
    set_short("Tolkeen, South Watch Platform");
    set_long(
        "A watch platform juts from the southern wall. Sentries scan the\n"
        "approach roads for Coalition scouts. A ley compass spins wildly here,\n"
        "useless until calibrated for this city's wards.");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_wall_n"
    ]) );
}
