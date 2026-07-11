/* /domains/Horton/areas/horton_wilderness_trail.c
   Packed dirt trail through scrub north of Horton outskirts. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Wilderness Trail");
    set_long(
        "A packed dirt trail winds through scrub and burned-out\n"
        "vehicle hulks. Tracks in the mud suggest large animals pass through\n"
        "regularly. The trail continues into the dark tree line.");
    set_exits( ([
        "south" : "/domains/Horton/areas/horton_outskirts",
        "north" : "/domains/Horton/areas/horton_abandoned_farmstead"
    ]) );
    set_listen("default",
        "Scratches in the underbrush. Wind through dead branches. Something moving.");
    set_smell("default",
        "Wet mud, rust, and animal musk.");
    set_items( ([
        "trail"    : "Packed dirt, worn by something heavy.",
        "hulks"    : "Burned-out pre-Rifts vehicles. Stripped for parts long ago.",
        "tracks"   : "Large paw prints in the mud. Recent.",
        "scrub"    : "Low thorny brush on both sides of the trail."
    ]) );
}

void reset() {
    object npc;

    if(!present("wild dog pack", this_object())) {
        npc = clone_object("/domains/Horton/monsters/wild_dog_pack");
        npc->move(this_object());
    }
    ::reset();
}
