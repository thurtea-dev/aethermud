// /domains/ChiTown/areas/cs_service_hatch.c
// Secret entrance between fortified city and maintenance tunnels.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("dark_room", 1);
    set_short("Chi-Town, Service Hatch");
    set_long(
        "A cramped utility shaft under the residential block. Conduits,\n"
        "damp concrete, and a ladder. This is how maintenance crews - and\n"
        "anyone who bribes them - move without using the gate.\n\n"
        "Up returns to the residential street. East opens into the\n"
        "maintenance tunnels.");
    set_exits( ([
        "up"   : "/domains/ChiTown/areas/cs_residential",
        "east" : "/domains/ChiTown/areas/chitown_maintenance_tunnel"
    ]) );
    set_listen("default", "Dripping water and distant tunnel echoes.");
    set_smell("default", "Wet concrete and hot insulation.");
}
