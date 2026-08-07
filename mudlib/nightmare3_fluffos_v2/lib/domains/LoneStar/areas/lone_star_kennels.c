/* /domains/LoneStar/areas/lone_star_kennels.c
   Lone Star Dog Boy kennel wing: the kennel rows proper.
   Where juvenile Dog Boys are housed before assignment.
   Connects south to the kennel yard. */

#define NPC_PATH "/domains/LoneStar/npcs/"
#define MON_PATH "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Kennel Rows");
    set_long(
        "Two long rows of pens face each other down a concrete aisle, each\n"
        "pen fronted with heavy mesh and numbered on a stenciled plate. The\n"
        "pens are clean and warm and entirely bare: a sleeping pad, a feed\n"
        "bowl, a drain in the floor. No lockers, no personal effects, no\n"
        "mural like the one in the barracks.\n\n"
        "Juveniles are housed here from the vats until they are assigned to\n"
        "a handler and moved out to the barracks block. Coalition doctrine\n"
        "calls this stage 'conditioning.'\n\n"
        "The kennel yard is back south.");
    set_exits( ([
        "south" : "/domains/LoneStar/areas/lone_star_kennel_yard"
    ]) );
    set_listen("default",
        "Claws on concrete, and the fast shallow breathing of something\n"
        "young pressed against the mesh, watching you pass.");
    set_smell("default",
        "Warm fur, disinfectant, and the mealy smell of ration paste.");
    set_items( ([
        "pens"    : "Heavy mesh fronts on concrete boxes, numbered rather\n"
                    "than named. Warm, clean, and bare.",
        "plates"  : "Stenciled number plates. The pens are numbered. So are\n"
                    "the occupants.",
        "pad"     : "A rubber sleeping pad, hosed down daily.",
        "bowl"    : "A bolted-down feed bowl. Ration paste, twice a shift.",
        "drain"   : "A floor drain running the length of the aisle. The\n"
                    "whole row can be washed out at once.",
        "aisle"   : "A concrete walkway between the two rows, wide enough\n"
                    "for a handler and a leash."
    ]) );
}

void reset() {
    object *inv;
    int count, i;

    ::reset();
    if(!present("kennel master", this_object()))
        clone_object(NPC_PATH + "ls_kennel_master.c")->move(this_object());
    count = 0;
    inv = all_inventory(this_object());
    for(i = 0; i < sizeof(inv); i++) {
        if(strsrch(base_name(inv[i]), "ls_dog_boy_pup") != -1)
            count++;
    }
    while(count < 2) {
        clone_object(MON_PATH + "ls_dog_boy_pup.c")->move(this_object());
        count++;
    }
}
