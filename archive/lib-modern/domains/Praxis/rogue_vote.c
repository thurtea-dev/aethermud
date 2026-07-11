#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Rogue voting nook");
    set_long(
        "A cramped alcove with a broken tally board. The war room lies east.");
    set_exits( ([
        "east" : "/domains/Praxis/rogue_hall"
    ]) );
}
