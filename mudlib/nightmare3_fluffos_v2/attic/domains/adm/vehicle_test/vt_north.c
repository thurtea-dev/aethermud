/* /domains/adm/vehicle_test/vt_north.c
   Vehicle Test Zone: north leg of the test loop. Staff only. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("vehicle_accessible", 1);
    set_property("ex5_accessible", 1);
    set_short("Vehicle Test Zone: North Corridor");
    set_long(
        "The north leg of the vehicle test corridor. Wide and flat,\n"
        "with lane markings painted on the permacrete floor.\n"
        "The corridor continues east and connects south to the staging bay.\n\n"
        "Exits: south (staging bay), east.");
    set_exits( ([
        "south" : "/domains/adm/vehicle_test/vt_main",
        "east"  : "/domains/adm/vehicle_test/vt_east"
    ]) );
}
