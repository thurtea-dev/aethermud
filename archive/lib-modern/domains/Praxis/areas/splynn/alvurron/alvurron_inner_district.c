// /domains/Praxis/areas/splynn/alvurron/alvurron_inner_district.c
// Deeper Alvurron, structural rather than decorative. High threat.

#include <std.h>
#include <rooms.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_short("Alvurron, Inner District");
    set_long(
        "The inner district is quieter than the plaza. The gargoyles here are\n"
        "larger and move with deliberate slowness. The architecture changes:\n"
        "less carving, more raw black stone. This part of the city is not\n"
        "meant for visitors. The temple lies west. A passage drops down into\n"
        "the deep stone.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/alvurron/alvurron_temple",
        "down" : "/domains/Praxis/areas/splynn/alvurron/alvurron_rift_chamber",
        "east" : "/domains/Praxis/areas/splynn/alvurron/alvurron_spire"
    ]) );
    set_listen("default",
        "Heavy footsteps on stone, evenly spaced, never hurried.");
    set_smell("default", "Ozone and cold mineral dust.");
}

void reset() {
    ::reset();
    if(!present("brodkil enforcer", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_enforcer")->move(this_object());
    if(!present("alvurron key", this_object()))
        clone_object(EQ_PATH + "alvurron_key")->move(this_object());
}
