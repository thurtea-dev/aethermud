// /d/Praxis/areas/long_road.c
// The Long Road: desolate highway connector between Praxis and the Splynn coast.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 1);
    set_property("ex5_accessible", 1);
    set_short("The Long Road");
    set_long(
        "A cracked pre-Rifts highway stretches east and west across a flat,\n"
        "blasted landscape. Rusted vehicles have been shoved to the shoulders\n"
        "by someone who needed the lane cleared. The Atlantic coastline is\n"
        "barely visible as a grey smudge to the east. Behind you to the west\n"
        "the towers of Praxis are dwindling.\n\n"
        "The road is wide enough for heavy vehicles. Tracks in the dust suggest\n"
        "something enormous passed through here not long ago.");
    set_exits( ([
        "west"  : "/domains/Praxis/areas/weapons_shop",
        "east"  : "/domains/Praxis/areas/splynn/splynn_market",
        "north" : "/domains/newcamelot/areas/europe_road"
    ]) );
    set_listen("default",
        "Wind across empty asphalt, and a distant mechanical groan from the east.");
    set_smell("default",
        "Sea salt, hot asphalt, and the chemical tang of something alien carried on the breeze.");
    set_items( ([
        "highway" : "Six cracked lanes running east toward the coast. Once a major artery.",
        "vehicles" : "Rusted husks pushed aside. Some are pre-Rifts makes. Others are not.",
        "tracks" : "Deep impressions in the dust, each one the size of a small building.",
        "coastline": "A distant grey line that might be water or might be ruin."
    ]) );
}

void reset() {
    ::reset();
}
