// /domains/tolkeen/areas/tolkeen_barracks.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_property("domain_stub", 1);
    set_short("Tolkeen, Militia Barracks");
    set_long(
        "Cots and weapon racks fill a reinforced hall. Tolkeen militia in\n"
        "patchwork MDC armor drill in the yard outside. Maps of Coalition\n"
        "advance lines cover one wall, updated in chalk daily.");
    set_exits( ([
        "east" : "/domains/tolkeen/areas/tolkeen_square"
    ]) );
}
