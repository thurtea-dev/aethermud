// /d/Praxis/areas/splynn/splynn_pits.c
// The Slave Pits: dark, dangerous. 2 aggressive Kittani warriors patrol here.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_property("vehicle_accessible", 0);
    set_short("Splynn Slave Pits");
    set_long(
        "A series of open pits cut into the stone, each one caged with\n"
        "energy bars that flicker between poles. The pits contain beings\n"
        "from multiple worlds, some huddled in groups, some motionless.\n"
        "The smell is overwhelming: unwashed bodies, waste, and fear.\n\n"
        "Two Kittani warriors patrol the pit perimeter. Their energy blades\n"
        "are drawn. They do not look welcoming.\n\n"
        "Exit: west back to the Splynn Dimensional Market.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/splynn_market",
        "south" : "/domains/Praxis/areas/splynn/slave_pens"
    ]) );
    set_listen("default",
        "Low moaning from the pits, the crackle of energy bars, and the\n"
        "measured footsteps of Kittani warriors on patrol.");
    set_smell("default",
        "Unwashed bodies, fear-sweat, and the ozone of energy containment fields.");
    set_items( ([
        "pits" : "Open pits in the stone floor, each one ten meters deep. The\n"
                        "energy bars make escape impossible without heavy MDC cutting tools.",
        "energy bars" : "Containment fields strung between poles. They crackle with\n"
                        "enough energy to reduce a human to ash.",
        "captives" : "Beings from a dozen worlds. Most are too far gone to make\n"
                        "eye contact. A few watch you with desperate hope.",
        "warriors" : "Two Kittani warriors in articulated power armor. They have\n"
                        "noticed you and their hands are on their weapons."
    ]) );
}

void reset() {
    ::reset();
    if(!present("kittani warrior", this_object())) {
        clone_object(MONSTER_PATH+"kittani_warrior.c")->move(this_object());
        clone_object(MONSTER_PATH+"kittani_warrior.c")->move(this_object());
    }
}
