// /domains/chitown/areas/burb_smuggler_crawl.c
// Secret crawl under/along the wall into maintenance tunnels.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("dark_room", 1);
    set_short("Smuggler Crawl");
    set_long(
        "A tight concrete tube, wet and low. Knees scrape. Someone chalked\n"
        "arrows on the wall years ago; most are smeared. This is how small\n"
        "packages - and smaller people - bypass the gate.\n\n"
        "The grate opens back to the wall's shadow. Ahead, the crawl empties\n"
        "into a maintenance tunnel under the city side.");
    set_exits( ([
        "out"  : "/domains/chitown/areas/burb_wall_shadow",
        "east" : "/domains/chitown/areas/chitown_maintenance_tunnel"
    ]) );
    set_listen("default",
        "Dripping water and your own breathing.");
    set_smell("default",
        "Mold, wet rust, and old smoke.");
    set_items( ([
        "arrows" : "Chalk marks. Some point east. Some point nowhere useful.",
        "tube" : "Barely wide enough for a human with a backpack."
    ]) );
}
