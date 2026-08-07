/* /domains/LoneStar/areas/lone_star_bunkroom.c
   Lone Star Dog Boy NCO bunkroom, off the main barracks hall.
   Smaller overflow quarters for senior guard personnel.
   Connects south to the barracks, east to the armory. */

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: NCO Bunkroom");
    set_long(
        "A smaller room off the main barracks hall, reserved for senior\n"
        "guard personnel. Four bunks instead of the rows outside, and a\n"
        "little more space between them. A locked door on the far wall\n"
        "leads deeper into the complex.\n\n"
        "The main barracks hall is south. The armory door is east.");
    set_exits( ([
        "south" : "/domains/LoneStar/areas/lone_star_barracks",
        "east"  : "/domains/LoneStar/areas/lone_star_armory"
    ]) );
    set_listen("default",
        "Quieter here than the main hall. The occasional creak of a bunk frame.");
    set_smell("default",
        "The same canine musk as the barracks, fainter, mixed with gun oil.");
    set_items( ([
        "bunks" : "Four bunks, better spaced than the rows in the main hall.",
        "door"  : "A locked steel door leading deeper into the complex,\n"
                  "toward the armory."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Dog Boy guard", this_object()))
        clone_object(NPC_PATH + "cs_dog_boy_guard.c")->move(this_object());
}
