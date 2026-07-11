// /domains/chitown/areas/burb_wall_shadow.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_short("In the Wall's Shadow");
    set_long(
        "The ferrocrete wall of Chi-Town rises sheer here, blotting the sun.\n"
        "Refuse piles against the base. A drainage grate sits half-hidden\n"
        "behind scrap - too small for a vehicle, maybe large enough for a\n"
        "desperate person.\n\n"
        "East returns to the tenements. Up reaches the rooftops. The grate\n"
        "leads into a smuggler crawl.");
    set_exits( ([
        "east"  : "/domains/chitown/areas/burb_tenements",
        "up"    : "/domains/chitown/areas/burb_rooftops",
        "grate" : "/domains/chitown/areas/burb_smuggler_crawl"
    ]) );
    set_listen("default",
        "Wall speakers murmuring Coalition slogans, muffled by distance.");
    set_smell("default",
        "Damp concrete, trash, and ozone from the wall defenses.");
    set_items( ([
        "wall" : "Smooth ferrocrete. Climbing it without gear is suicide.",
        "grate" : "A drainage grate pried loose often enough that the bolts are gone.",
        "refuse" : "The city's leftovers, pushed outside where nobody official looks."
    ]) );
}
