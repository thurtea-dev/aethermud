// /domains/chitown/areas/burb_flophouse.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("rest_allowed", 1);
    set_short("Burb Flophouse");
    set_long(
        "A single large room lined with bunks and hanging blankets for\n"
        "privacy that does not exist. The floor is sticky. A hand-lettered\n"
        "board lists nightly rates in credits.\n\n"
        "The door south opens onto the soup kitchen.");
    set_exits( ([
        "south" : "/domains/chitown/areas/burb_soup_kitchen"
    ]) );
    set_listen("default",
        "Snoring, whispered deals, a radio turned very low.");
    set_smell("default",
        "Unwashed bedding and cheap disinfectant.");
    set_items( ([
        "bunks" : "Narrow, stacked, and occupied more often than not.",
        "board" : "5 credits a night. 20 for a corner with fewer fleas."
    ]) );
}
