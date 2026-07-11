#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Among the willow roots");
    set_long(
        "Twisted roots form a low crawlspace beneath the ley-scarred willow.\n"
        "Stairs lead up into the buried mage sanctum.");
    set_exits( ([
        "up" : "/domains/Praxis/mage_hall"
    ]) );
}
