/* /domains/NGR/areas/ngr_frontline_trench.c
   NGR Front, Communications Trench. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("NGR Front, Communications Trench");
    set_long(
        "A deep communications trench cut in zigzags toward the forward\n"
        "line, walls faced with plascrete panels and duckboards underfoot.\n"
        "Field telephone wire staples along one wall. Every corner has a\n"
        "firing step and a painted range card. The watchtower is west.\n"
        "The trench opens east onto no man's land.");
    set_exits( ([
        "west" : "/domains/NGR/areas/ngr_watchtower",
        "east" : "/domains/NGR/areas/ngr_no_mans_land"
    ]) );
    set_listen("default",
        "Boots on duckboards and a field telephone winding somewhere.");
    set_smell("default", "Wet plascrete, mud, and tobacco.");
    set_items( ([
        "duckboards" : "Keeping boots out of the water that seeps in anyway.",
        "wire" : "Field telephone line, stapled and tagged by section.",
        "range cards" : "Painted distances to every landmark a gargoyle might
stand next to."
    ]) );
}

void reset() {
    ::reset();
    if(!present("elite soldier", this_object()))
        clone_object("/domains/NGR/npcs/ngr_elite_soldier")->move(this_object());
}
