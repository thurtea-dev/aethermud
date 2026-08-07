#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Mage order voting room");
    set_long(
        "Stone benches circle a dead brazier. The old mage council voted here\n"
        "on matters of Praxis. The sanctum lies east.");
    set_exits( ([
        "east" : "/domains/Praxis/mage_hall"
    ]) );
}
