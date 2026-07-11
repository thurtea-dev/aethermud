// /domains/tolkeen/areas/tolkeen_refugee_yard.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_property("rest_allowed", 1);
    set_short("Tolkeen, Refugee Yard");
    set_long(
        "Tents pressed against the inner wall. Cookfires. Children. A board\n"
        "lists work details: wall repair, kitchen, crypt diggers.\n\n"
        "The ward market is north.");
    set_exits( ([
        "north" : "/domains/tolkeen/areas/tolkeen_market"
    ]) );
    set_listen("default", "Quiet talk and a crying infant.");
    set_smell("default", "Smoke and thin stew.");
}

void reset() {
    ::reset();
    if(!present("tolkeen refugee", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_refugee")->move(this_object());
}
