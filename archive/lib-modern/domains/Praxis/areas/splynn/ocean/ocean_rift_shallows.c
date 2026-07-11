/* /domains/Praxis/areas/splynn/ocean/ocean_rift_shallows.c
   Ocean Rift, Shallows: first underwater zone below the Splynn ocean rift. */

#include <std.h>
#include <rooms.h>

#define MON_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("water", 1);
    set_property("rest_allowed", 0);
    set_short("Ocean Rift, Shallows");
    set_long(
        "Fractured light filters down through water stained with rift energy,\n"
        "blues and golds that do not come from the sun. The seafloor is not\n"
        "visible from here. Strange currents pull at you from below, patient\n"
        "and cold.\n\n"
        "Non-aquatic beings require a breathing apparatus or the Swim as a\n"
        "Fish spell to survive extended time in these waters.\n"
    );
    set_exits( ([
        "up"   : "/domains/Praxis/areas/splynn/ocean_rift",
        "down" : "/domains/Praxis/areas/splynn/ocean/ocean_rift_deep"
    ]) );
    set_listen("default",
        "The pressure of water and the low hum of the dimensional rift below.");
    set_smell("default",
        "Salt, rift ozone, and the mineral smell of deep ocean.");
    set_items( ([
        "water" : "Rift-stained seawater, its colors not entirely natural. Moving\n"
                  "through it requires effort against the slow current pulling down.",
        "light" : "Filtered Atlantic sunlight mixed with rift energy. The blue and\n"
                  "gold tones are not from any natural source.",
        "current" : "A slow persistent pull from below. Something is drawing water\n"
                    "down toward the rift's deepest point."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ocean creature", this_object()))
        clone_object(MON_PATH + "ocean_creature")->move(this_object());
}
