// /domains/tolkeen/areas/tolkeen_inn_rooms.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_property("rest_allowed", 1);
    set_short("Tolkeen, Inn Upper Hall");
    set_long(
        "A hall of small rooms with thin doors. Curtains for privacy that\n"
        "does not exist. Ward alarms still audible through the floor.\n\n"
        "Stairs lead down to the common room.");
    set_exits( ([
        "down" : "/domains/tolkeen/areas/tolkeen_inn"
    ]) );
    set_listen("default", "Snoring and distant wall horns.");
    set_smell("default", "Dust and old blankets.");
}
