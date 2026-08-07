// /domains/ChiTown/areas/cs_dogboy_kennels.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Chi-Town, Dog Boy Kennels");
    set_long(
        "A row of reinforced stalls and a staging area for Dog Boy patrols.\n"
        "Water bowls. Weapon racks. A chalkboard lists shift assignments.\n"
        "The smell is unmistakable.\n\n"
        "The SAMAS hangar is south. Barracks access is west.");
    set_exits( ([
        "south" : "/domains/ChiTown/areas/cs_samas_hangar",
        "west"  : "/domains/ChiTown/areas/cs_barracks"
    ]) );
    set_listen("default", "Claws on ferrocrete. A low growl. Radio chatter.");
    set_smell("default", "Wet fur, disinfectant, and gun oil.");
    set_items( ([
        "chalkboard" : "Shift names and sectors. Most routes cover the burbs.",
        "stalls" : "Reinforced. Built for mutants who can tear steel."
    ]) );
}

void reset() {
    ::reset();
    if(!present("cs dog boy", this_object()))
        clone_object("/domains/ChiTown/npcs/cs_dog_boy")->move(this_object());
}
