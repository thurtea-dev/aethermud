// /domains/tolkeen/areas/tolkeen_square.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_property("domain_stub", 1);
    set_short("Tolkeen, Liberation Square");
    set_long(
        "A broad plaza paved with salvaged ferrocrete and ward stones. Banners\n"
        "show the Tolkeen crest: a fist clutching a lightning bolt. Mercenaries,\n"
        "mages, and refugees mix in the crowd. Coalition propaganda is burned\n"
        "wherever it appears.");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_gate",
        "east"  : "/domains/tolkeen/areas/tolkeen_market",
        "west"  : "/domains/tolkeen/areas/tolkeen_barracks",
        "north" : "/domains/tolkeen/areas/tolkeen_wall_n"
    ]) );
}
