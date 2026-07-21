// /domains/ChiTown/areas/burb_gang_corner.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_short("Gang Corner");
    set_long(
        "Spray-painted tags cover every vertical surface. A burned-out car\n"
        "blocks half the lane. This corner belongs to whoever is mean enough\n"
        "to hold it this week. The air feels watched.\n\n"
        "West returns to the water pump. South opens into a chop shop yard.\n"
        "North connects to the dark alley off the main burbs street.");
    set_exits( ([
        "west"  : "/domains/ChiTown/areas/burb_water_pump",
        "south" : "/domains/ChiTown/areas/burb_chop_shop",
        "north" : "/domains/ChiTown/areas/chitown_alley"
    ]) );
    set_listen("default",
        "Low laughter, a chain dragging on concrete, distant sirens.");
    set_smell("default",
        "Burned rubber, cheap liquor, and fear-sweat.");
    set_items( ([
        "tags" : "Gang marks layered over older marks. Territory changes often.",
        "car" : "A pre-Rifts shell, stripped of anything useful years ago."
    ]) );
}

void reset() {
    ::reset();
    if(!present("burb thug", this_object()))
        clone_object("/domains/ChiTown/monsters/burb_thug")->move(this_object());
}
