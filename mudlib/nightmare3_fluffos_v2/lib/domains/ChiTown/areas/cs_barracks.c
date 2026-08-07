// /domains/ChiTown/areas/cs_barracks.c
// CS Barracks. Training and housing for Coalition soldiers.

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/ChiTown/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no steal", 1);
    set_short("Chi-Town, CS Barracks");
    set_long(
        "Rows of metal bunks stretch down a long hall lit by harsh white\n"
        "panels. Footlockers sit at attention beneath each rack. Weapon pegs\n"
        "line the walls, most of them empty while soldiers are on duty. A\n"
        "drill floor at the far end shows scuff marks from endless formations.\n\n"
        "The command plaza is west. The recruitment center is north.\n"
        "A secured stairwell leads up to the officer wing. Dog Boy kennels\n"
        "are east.");
    set_exits( ([
        "west"  : "/domains/ChiTown/areas/cs_command_plaza",
        "north" : "/domains/ChiTown/areas/cs_recruitment",
        "up"    : "/domains/ChiTown/areas/cs_secure_wing",
        "east"  : "/domains/ChiTown/areas/cs_dogboy_kennels"
    ]) );
    set_listen("default",
        "Clipped footsteps, locker latches, and a distant drill instructor's bark.");
    set_smell("default",
        "Boot polish, canvas, and stale institutional air.");
    set_items( ([
        "bunks" : "Metal frame bunks with tight gray blankets.",
        "lockers" : "Footlockers stamped with CS serial numbers.",
        "pegs" : "Weapon pegs on the wall. Most are bare.",
        "floor" : "Scuffed drill floor. Endless formations wore it smooth."
    ]) );
}

void reset() {
    ::reset();
    if(!present("dead boy guard", this_object()))
        clone_object(NPC_PATH + "dead_boy_guard")->move(this_object());
    if(!present("cs recruiter", this_object()))
        clone_object(NPC_PATH + "cs_recruiter")->move(this_object());
    if(!present("cs juicer", this_object()) && !random(2))
        clone_object("/domains/ChiTown/monsters/cs_juicer")->move(this_object());
}
