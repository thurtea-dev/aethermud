/* /domains/LoneStar/areas/lone_star_armory.c
   Lone Star weapons lockup, beyond the NCO bunkroom.
   Connects west to the bunkroom. */

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Lone Star: Armory");
    set_long(
        "Weapon racks line every wall, bolted down and secured with\n"
        "numbered locks. Standard Coalition rifles, sidearms, and a rack of\n"
        "riot gear take up most of the floor space. A sign-out ledger sits\n"
        "on a podium by the door, chained to the wall.\n\n"
        "The bunkroom is back west.");
    set_exits( ([
        "west" : "/domains/LoneStar/areas/lone_star_bunkroom"
    ]) );
    set_listen("default",
        "The faint metallic settle of racked weapons. Nothing else moves in here.");
    set_smell("default",
        "Gun oil and cold metal.");
    set_items( ([
        "racks"   : "Weapon racks bolted to every wall, each rifle and\n"
                    "sidearm secured with a numbered lock.",
        "ledger"  : "A sign-out ledger chained to a podium by the door.\n"
                    "Every withdrawal is logged, name and time.",
        "riot gear" : "A rack of riot gear: shields, helmets, restraint\n"
                    "cuffs. Standard issue for a containment breach."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Dog Boy guard", this_object()))
        clone_object(NPC_PATH + "cs_dog_boy_guard.c")->move(this_object());
}
