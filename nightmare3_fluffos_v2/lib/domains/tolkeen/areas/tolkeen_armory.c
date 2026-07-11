// /domains/tolkeen/areas/tolkeen_armory.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Militia Armory");
    set_long(
        "Locked racks of rifles, vibro-blades, and TW charms. A quartermaster\n"
        "desk sits empty - stock is issued, not sold, unless you know someone.\n\n"
        "The barracks are south.");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_barracks"
    ]) );
    set_listen("default", "Quiet. A lock ticking as it cools.");
    set_smell("default", "Gun oil.");
}
