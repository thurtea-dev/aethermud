/* /domains/NGR/areas/ngr_gargoyle_ridge.c
   Gargoyle Ridge, named from No Man's Land but never built until now. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_short("Gargoyle Ridge");
    set_long(
        "The ground climbs into broken rock, worn smooth in patches where\n"
        "wings and claws have used the same perches for longer than the\n"
        "NGR has held its line below. This is not contested ground. This\n"
        "is theirs, and every sightline down toward No Man's Land makes\n"
        "that clear.\n\n"
        "Bones, human and otherwise, are wedged into the rock crevices,\n"
        "old kills kept rather than discarded. No Man's Land lies back\n"
        "west, down the slope.");
    set_exits( ([
        "west" : "/domains/NGR/areas/ngr_no_mans_land"
    ]) );
    set_listen("default",
        "Wind through broken rock, and the scrape of claws shifting on\n"
        "stone somewhere above.");
    set_smell("default",
        "Old blood gone to dust, and the dry mineral smell of sun-baked\n"
        "rock.");
    set_items( ([
        "rock"     : "Broken rock worn smooth in patches by generations of\n"
                     "wings and claws using the same perches.",
        "bones"    : "Wedged into crevices throughout the ridge. Kept, not\n"
                     "discarded. Some are recent.",
        "perches"  : "Weathered stone shelves along the rock face, each one\n"
                     "worn to the same shape by the same use.",
        "slope"    : "The way back down to No Man's Land, west and below."
    ]) );
}

void reset() {
    ::reset();
    if(!present("gargoyle raider", this_object()))
        clone_object("/domains/NGR/npcs/gargoyle_raider")->move(this_object());
    if(!present("gargoyle raider 2", this_object()))
        clone_object("/domains/NGR/npcs/gargoyle_raider")->move(this_object());
}
