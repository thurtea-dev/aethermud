#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Buried mage sanctum");
    set_long(
        "A circular chamber under the willow, lined with cracked rune stones.\n"
        "Old spell components crumble on shelves. The mage order that met here\n"
        "did not survive the first rift storms. Stairs lead up to the hollow\n"
        "trunk. A voting room lies west.");
    set_exits( ([
        "up"    : "/domains/Praxis/mage_join",
        "west"  : "/domains/Praxis/mage_vote"
    ]) );
}
