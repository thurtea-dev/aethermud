/* /d/Praxis/vehicles/mountaineer/mtn_cabin.c
   NG-V7 Mountaineer ATV: main cabin and entry room.
   Entry from east_road2; north to cab, south to bunks, east to cargo. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("vehicle_interior", 1);
    set_short("NG-V7 Mountaineer: Main Cabin");
    set_long(
        "The main cabin of a Northern Gun NG-V7 Mountaineer, an armored\n"
        "all-terrain RV built for months in the wilderness. A fold-down\n"
        "table is bolted between two bench seats, and overhead lockers\n"
        "line both walls. A compact galley unit with a two-burner stove\n"
        "and water recycler is set into the aft bulkhead.\n\n"
        "The driver's cab is forward (north). A narrow passage aft leads\n"
        "to the bunk room (south). A side hatch opens into the cargo hold\n"
        "(east). The boarding door is beside you. Type 'out' to climb down\n"
        "to East Road.");
    set_exits( ([
        "out"   : "/domains/Praxis/east_road2",
        "north" : "/domains/Praxis/vehicles/mountaineer/mtn_cab",
        "south" : "/domains/Praxis/vehicles/mountaineer/mtn_bunks",
        "east"  : "/domains/Praxis/vehicles/mountaineer/mtn_cargo"
    ]) );
    set_listen("default",
        "The tick of a cooling engine and wind against the armored hull.");
    set_smell("default",
        "Coffee, gun oil, and recycled air.");
    set_items( ([
        "table" : "A fold-down mess table, scarred by knife games and hot pans.",
        "lockers" : "Overhead storage lockers. Most hang open and empty.",
        "galley" : "A compact galley: two-burner stove, sink, and a water\n"
                   "recycler rated for six crew.",
        "benches" : "Two bench seats with cracked padding, bolted to the deck.",
        "door" : "The armored boarding door, currently open. Type 'out' to leave."
    ]) );
}

void reset() {
    ::reset();
}
