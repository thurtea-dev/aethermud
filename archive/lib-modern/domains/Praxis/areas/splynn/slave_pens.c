// /d/Praxis/areas/splynn/slave_pens.c
// Slave pen holding area south of the Slave Pits. Stub.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Slave Pens, Lower Level");
    set_long(
        "The lower level of the slave holding facility: a long corridor of\n"
        "individual cells cut into the stone. Each cell is sealed with an\n"
        "energy lattice. The air is stale and the lighting is minimal.\n\n"
        "The captives here are separated from those in the upper pits.\n"
        "These are the ones the Splugorth consider particularly valuable,\n"
        "or particularly dangerous. Some of them watch you with eyes that\n"
        "have seen too much. Others do not look up.\n\n"
        "This facility runs deep. There may be more levels below,\n"
        "but the access is sealed.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/splynn_pits"
    ]) );
    set_listen("default",
        "The hum of containment lattices. Quiet that feels enforced.\n"
        "Somewhere, someone is breathing in short rapid bursts.");
    set_smell("default",
        "Stale recycled air with undertones of chemical sedative and\n"
        "the particular smell of long-term confinement.");
    set_items( ([
        "cells" : "Individual cells sealed with energy lattices. The technology\n"
                     "is Splugorth-made and rated far above what any captive here could break.",
        "captives" : "Beings from multiple worlds and dimensions. The Splugorth\n"
                     "select for value: psionics, magic-users, rare bloodlines.\n"
                     "Some look at you. Some do not.",
        "lattice" : "Energy containment lattice across each cell entrance.\n"
                     "Brushing it would be a very bad idea.",
        "corridor" : "Long and low-ceilinged. The stone is damp. The cells go\n"
                     "further than the light reaches."
    ]) );
}

void reset() {
    object *here;
    int i;
    int slaves;

    ::reset();
    here = all_inventory(this_object());
    slaves = 0;
    for(i = 0; i < sizeof(here); i++) {
        if(here[i] && (int)here[i]->query_property("is_slave"))
            slaves++;
    }
    while(slaves < 2) {
        clone_object("/domains/Praxis/monsters/splynn_slave")->move(this_object());
        slaves++;
    }
}
