/* /domains/Praxis/areas/splynn/ocean/ocean_rift_deep.c
   Ocean Rift, Deep Zone: home of the ocean leviathan. */

#include <std.h>
#include <rooms.h>

#define MON_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("water", 1);
    set_property("rest_allowed", 0);
    set_short("Ocean Rift, Deep Zone");
    set_long(
        "Complete darkness except for the faint blue glow of the rift tear\n"
        "below. The pressure at this depth is crushing. Water moves in slow\n"
        "spirals around a dimensional distortion that has no natural cause.\n\n"
        "Something large moves in the dark. You can hear it: displacement of\n"
        "water in rhythms too slow and too regular to be current.\n\n"
        "The rift floor is faintly visible below, lit by cold light from\n"
        "structures that should not exist this deep in the Atlantic. Non-aquatic\n"
        "beings require full environmental protection to survive this depth.\n"
    );
    set_exits( ([
        "up"   : "/domains/Praxis/areas/splynn/ocean/ocean_rift_shallows",
        "down" : "/domains/Praxis/areas/splynn/ocean/ocean_rift_floor"
    ]) );
    set_listen("default",
        "Slow rhythmic displacement in the dark. Something very large, breathing\n"
        "or moving through water in long, deliberate cycles.");
    set_smell("default",
        "The cold mineral smell of the deep Atlantic. Old water. Old darkness.");
    set_items( ([
        "darkness" : "Total. Only the blue rift glow below gives any reference.\n"
                     "Your eyes are useless here without light equipment.",
        "glow" : "Blue-white light from the rift tear below, illuminating the\n"
                 "edges of ancient stonework on the ocean floor.",
        "movement" : "Whatever it is, it is large. The displacement of water when\n"
                     "it moves is perceptible as pressure changes against your body."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ocean leviathan", this_object()))
        clone_object(MON_PATH + "ocean_leviathan")->move(this_object());
}
