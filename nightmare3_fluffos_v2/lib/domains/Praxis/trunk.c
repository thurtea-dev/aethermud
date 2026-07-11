#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("Willow trunk platform");
    set_long(
        "A platform built in the fork of the ancient willow. Rope ladder leads\n"
        "down toward the buried sanctum. The forest road is below.");
    set_exits( ([
        "down" : "/domains/Praxis/mage_hall"
    ]) );
}
