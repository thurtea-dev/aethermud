// /domains/ChiTown/areas/burb_refugee_camp.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("rest_allowed", 1);
    set_short("Refugee Tent Camp");
    set_long(
        "A scatter of tents and lean-tos along Illinois Road's shoulder.\n"
        "New arrivals wait here for news of the gate, work, or a ride that\n"
        "never comes. A faded sign points north toward the burbs proper.\n\n"
        "Illinois Road is south. West leads into deep burb row.");
    set_exits( ([
        "south" : "/domains/ChiTown/areas/illinois_road",
        "west"  : "/domains/ChiTown/areas/chitown_burb_row"
    ]) );
    set_listen("default",
        "Quiet talk, a child coughing, wind on tent canvas.");
    set_smell("default",
        "Woodsmoke, damp cloth, and road dust.");
    set_items( ([
        "tents" : "Military surplus and homemade. None look waterproof.",
        "sign" : "CHI-TOWN BURBS - KEEP MOVING unless you like mud."
    ]) );
}

void reset() {
    ::reset();
    if(!present("burb refugee", this_object()))
        clone_object("/domains/ChiTown/npcs/burb_refugee")->move(this_object());
}
