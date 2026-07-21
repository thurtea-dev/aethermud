// /domains/NewCamelot/areas/black_forest.c
// Dense magical forest north of Europe Road. A ley line nexus glows overhead.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("ley_line_nexus", 1);
    set_property("ley_line_room", 1);
    set_property("is_ley_line", 1);
    set_short("Black Forest Ley Line Nexus");
    set_long(
        "Dense old-growth forest so thick the sky is barely visible. What\n"
        "light reaches the floor is filtered green and strange. Overhead,\n"
        "two ley lines cross in a visible X of blue-white energy. The nexus\n"
        "point pulses with each passing minute.\n\n"
        "The energy here is immense. Magical beings feel it immediately, a\n"
        "pull at the center of their awareness. The ground itself thrums.\n\n"
        "Something has been here recently. Scorched earth in a rough circle\n"
        "beneath the nexus point. A smooth stone rests in the center.\n"
    );
    set_exits( ([
        "north" : "/domains/NewCamelot/areas/europe_road",
        "south" : "/domains/NewCamelot/areas/forest_lake"
    ]) );
    set_listen("default",
        "A constant low harmonic hum from the ley line intersection overhead.");
    set_smell("default",
        "Ozone, damp earth, and the sharp clean smell of raw magical energy.");
    set_items( ([
        "ley lines" : "Two ribbons of blue-white energy crossing overhead in a bright X.",
        "nexus" : "The crossing point of the two ley lines. The ground beneath it is scorched.",
        "trees" : "Ancient trees, their bark etched with patterns that might be natural. Might not be.",
        "mist" : "Glowing mist pooling around the root systems. Not natural fog.",
        "scorched" : "Scorched earth in a rough circle beneath the nexus point. Recent.",
        "earth" : "The scorched ground beneath the nexus. Warm to the touch."
    ]) );
}

void reset() {
    object stone;

    ::reset();
    if(!present("nexus stone", this_object())) {
        stone = clone_object("/domains/NewCamelot/equipment/nexus_stone");
        if(stone) stone->move(this_object());
    }
}
