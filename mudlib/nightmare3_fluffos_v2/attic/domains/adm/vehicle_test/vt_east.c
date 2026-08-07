/* /domains/adm/vehicle_test/vt_east.c
   Vehicle Test Zone: east leg of the test loop. Staff only. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("vehicle_accessible", 1);
    set_property("ex5_accessible", 1);
    set_short("Vehicle Test Zone: East Corridor");
    set_long(
        "The east leg of the vehicle test corridor. Same permacrete\n"
        "construction and lane markings as the rest of the loop.\n\n"
        "Exits: north, south.");
    set_exits( ([
        "north" : "/domains/adm/vehicle_test/vt_north",
        "south" : "/domains/adm/vehicle_test/vt_south"
    ]) );
}
