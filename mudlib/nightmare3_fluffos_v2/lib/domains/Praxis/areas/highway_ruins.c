// /d/Praxis/areas/highway_ruins.c
// Further north on old Highway 666: rusted vehicles, strange energy readings.

#include <std.h>
#include <rooms.h>

#define MON_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 1);
    set_short("Highway 666: Ruins Section");
    set_long(
        "Half a dozen pre-Rifts vehicles lie scattered across the highway lanes,\n"
        "rusted into shapes that barely suggest their original purpose. One still\n"
        "has its frame intact, a heavy transport truck tilted onto its side,\n"
        "cargo doors yawning open. Whatever it carried was taken long ago.\n\n"
        "The asphalt here is fractured differently than further south. The cracks\n"
        "radiate from a central point and glow faint blue-white in the dark. This\n"
        "is where a ley line surfaced once, or still does, intermittently. The\n"
        "energy readings would make a Ley Line Walker weep. Or laugh.\n\n"
        "The road continues north toward what looks like an active ley line crossing.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/old_highway",
        "north" : "/domains/Praxis/areas/nexus_approach",
        "east"  : "/domains/Lazlo/areas/lazlo_gate"
    ]) );
    set_listen("default",
        "A low electrical hum from the cracks in the asphalt. The hum is not constant.");
    set_smell("default",
        "Ozone, rust, and something else: ionized air from dimensional leakage.");
    set_items( ([
        "truck" : "A pre-Rifts heavy transport, rusted to a deep orange. The cargo hold is empty.",
        "vehicles" : "Six rusted vehicles in various states of decay. No bodies. Whatever happened here was a long time ago.",
        "cracks" : "Radiating fractures in the asphalt, glowing faint blue-white. An old ley line surfacing point.",
        "glow" : "Blue-white energy in the asphalt cracks. Magical residue from ley line activity.",
        "asphalt" : "Cracked and fractured. The energy readings here would register high on any magical scanner.",
        "cargo" : "The truck cargo hold is empty and has been for years. Scavengers got here first."
    ]) );
}

void reset() {
    int i;
    ::reset();
    if(!present("highway bandit", this_object())) {
        i = 1 + random(2);
        while(i-- > 0)
            clone_object(MON_PATH+"highway_bandit.c")->move(this_object());
    }
    if(!present("D-Bee wanderer", this_object()) && !random(3))
        clone_object(MON_PATH+"d_bee_wanderer.c")->move(this_object());
}
