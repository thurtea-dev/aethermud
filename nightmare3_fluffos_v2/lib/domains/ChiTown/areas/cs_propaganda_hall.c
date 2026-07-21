// /domains/ChiTown/areas/cs_propaganda_hall.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Chi-Town, Propaganda Hall");
    set_long(
        "A public hall lined with screens. Silent footage of Coalition\n"
        "victories loops forever. Benches face the largest display. Citizens\n"
        "sit. Nobody speaks loudly.\n\n"
        "Officer Row is east.");
    set_exits( ([
        "east" : "/domains/ChiTown/areas/chitown_officer_row"
    ]) );
    set_listen("default", "Almost silent. Soft chair creaks.");
    set_smell("default", "Floor polish.");
    set_items( ([
        "screens" : "HUMANITY FIRST. THE STRONG SURVIVE. Endless victory loops.",
        "benches" : "Hard plastic. Built for endurance, not comfort."
    ]) );
}

void reset() {
    ::reset();
    if(!present("chi-town citizen", this_object()))
        clone_object("/domains/ChiTown/npcs/cs_citizen")->move(this_object());
}
