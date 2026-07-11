// /domains/tolkeen/areas/tolkeen_wall_n.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_property("domain_stub", 1);
    set_short("Tolkeen, North Wall Walk");
    set_long(
        "You walk the northern battlements. Ward pylons line the parapet.\n"
        "Through the haze you can see ley storms rolling across the wilds\n"
        "beyond Tolkeen's claimed territory.");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_square",
        "north" : "/domains/tolkeen/areas/tolkeen_wall_s"
    ]) );
}
