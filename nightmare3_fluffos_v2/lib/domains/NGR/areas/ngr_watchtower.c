/* /domains/NGR/areas/ngr_watchtower.c
   NGR Front, Watchtower. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("NGR Front, Watchtower");
    set_long(
        "A steel watchtower rises over the eastern end of the patrol road,\n"
        "its legs sandbagged and its platform bristling with sensor gear.\n"
        "From here the land drops toward the front: wire, craters, and the\n"
        "far ridge where the gargoyles hold. A sniper keeps the long watch.\n"
        "The patrol road is west. A communications trench runs east.");
    set_exits( ([
        "west" : "/domains/NGR/areas/ngr_patrol_road",
        "east" : "/domains/NGR/areas/ngr_frontline_trench"
    ]) );
    set_listen("default",
        "Wind against guy-wires and the click of a sensor mast turning.");
    set_smell("default", "Cold mud and cordite.");
    set_items( ([
        "tower" : "Steel legs, sandbags, and a platform of sensor masts.",
        "wire" : "Concertina wire in triple rows, sagging where things have
come through.",
        "ridge" : "The gargoyle line. It does not move much. Neither does
anyone here."
    ]) );
}

void reset() {
    ::reset();
    if(!present("sniper", this_object()))
        clone_object("/domains/NGR/npcs/ngr_sniper")->move(this_object());
}
