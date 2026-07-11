/* /domains/NGR/areas/ngr_no_mans_land.c
   No Man's Land. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_short("No Man's Land");
    set_long(
        "The dead ground between the NGR line and the gargoyle ridge.\n"
        "Craters overlap craters. Burned-out power armor lies half sunk in\n"
        "the mud, and the wire here is torn into useless curls. Nothing\n"
        "grows. Things hunt here at all hours. The trench is west.");
    set_exits( ([
        "west" : "/domains/NGR/areas/ngr_frontline_trench"
    ]) );
    set_listen("default",
        "Distant artillery, and closer, the beat of heavy wings.");
    set_smell("default", "Mud, rust, and old burning.");
    set_items( ([
        "craters" : "Overlapping, water-filled, uncounted.",
        "wreckage" : "A burned X-10, face down. Nobody has retrieved the
pilot. Nobody is going to.",
        "wire" : "Torn concertina, curled like burnt hair."
    ]) );
}

void reset() {
    ::reset();
    if(!present("gargoyle raider", this_object()))
        clone_object("/domains/NGR/npcs/gargoyle_raider")->move(this_object());
    if(!present("gargoyle raider 2", this_object()))
        clone_object("/domains/NGR/npcs/gargoyle_raider")->move(this_object());
}
