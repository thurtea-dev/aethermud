// /d/Praxis/vehicles/ex5/ex5_forward_bay.c
// EX-5 Behemoth -- forward weapons bay.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_property("vehicle_interior", 1);
    set_property("vehicle_daemon", "/daemon/ex5_d");
    set_short("EX-5 Behemoth: Forward Weapons Bay");
    set_long(
        "The forward weapons bay fills the nose of the EX-5. Banks of\n"
        "inactive weapon systems line both walls: rail accelerators, particle\n"
        "beam projectors, and missile launch tubes that run the width of the\n"
        "room. Everything is offline: cold, dark, no charge indicators.\n\n"
        "A maintenance terminal is bolted to the forward bulkhead, its screen\n"
        "dark. Someone has scratched graffiti into the terminal housing:\n"
        "PROPERTY OF NARUNI ENTERPRISES. Below that, in different handwriting:\n"
        "NOT ANYMORE.\n\n"
        "Exit: south back to the main hull. Up leads to the cockpit.");
    set_exits( ([
        "south" : "/domains/Praxis/vehicles/ex5/ex5_hull",
        "up" : "/domains/Praxis/vehicles/ex5/ex5_cockpit"
    ]) );
    set_listen("default",
        "Silence. These weapons systems have not made a sound in a very long time.");
    set_smell("default",
        "Propellant residue, cold metal, and something that might be ozone\n"
        "from a particle beam that last fired years ago.");
    set_items( ([
        "weapons" : "Banks of rail accelerators, particle beam projectors, and missile\n"
                     "tubes. All offline. All waiting.",
        "rail" : "Rail accelerator barrels the length of the room. The acceleration\n"
                     "coils are cold but intact.",
        "particle" : "Particle beam projectors mounted on articulated arms. The optics\n"
                     "are clean. Someone maintained these.",
        "missiles" : "Missile launch tubes. The bay doors are closed. Unknown if loaded.",
        "terminal" : "A maintenance terminal with a dark screen. The graffiti on the\n"
                     "housing has a point: this machine is no longer anyone's property.",
        "graffiti" : "PROPERTY OF NARUNI ENTERPRISES. Below that: NOT ANYMORE."
    ]) );
}

void reset() {
    ::reset();
}
