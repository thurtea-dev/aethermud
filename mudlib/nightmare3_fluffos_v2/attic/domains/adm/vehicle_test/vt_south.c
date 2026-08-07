/* /domains/adm/vehicle_test/vt_south.c
   Vehicle Test Zone: south leg of the test loop. Staff only. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("vehicle_accessible", 1);
    set_property("ex5_accessible", 1);
    set_short("Vehicle Test Zone: South Corridor");
    set_long(
        "The south leg of the vehicle test corridor, completing the loop.\n"
        "Lane markings guide vehicles back toward the staging bay.\n\n"
        "Exits: east, west (staging bay).");
    set_exits( ([
        "east"  : "/domains/adm/vehicle_test/vt_east",
        "west"  : "/domains/adm/vehicle_test/vt_main"
    ]) );
}
