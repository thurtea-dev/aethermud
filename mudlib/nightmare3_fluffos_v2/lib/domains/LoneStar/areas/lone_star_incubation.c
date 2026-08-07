/* /domains/LoneStar/areas/lone_star_incubation.c
   Lone Star incubation gallery, north of the containment wing.
   The floor of the vat hall where Dog Boy lines are actually grown.
   Connects south to containment, west to cold storage, up to the
   observation booth. */

#define NPC_PATH "/domains/LoneStar/npcs/"
#define MON_PATH "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Incubation Gallery");
    set_long(
        "The gallery runs two stories high, and both of them are vats. Rows\n"
        "of upright growth tanks stand shoulder to shoulder down the length\n"
        "of the hall, lit from below so the shapes inside show as silhouettes\n"
        "against pale amber fluid. Feed lines and monitor cabling hang in\n"
        "bundles from the ceiling and run down the back of every tank.\n\n"
        "This is where the Coalition actually makes Dog Boys. Not a metaphor,\n"
        "not a program on paper: a production floor with a shift schedule and\n"
        "a yield target chalked on the wall.\n\n"
        "The containment wing is south. Cold storage is west, behind an\n"
        "insulated door. A steel stair climbs up to the observation booth.");
    set_exits( ([
        "south" : "/domains/LoneStar/areas/lone_star_containment",
        "west"  : "/domains/LoneStar/areas/lone_star_cold_storage",
        "up"    : "/domains/LoneStar/areas/lone_star_observation"
    ]) );
    set_listen("default",
        "Pumps cycling in sequence down the row, and a slow irregular\n"
        "thud from inside one of the tanks that does not match the pumps.");
    set_smell("default",
        "Warm amniotic fluid and hot electrical insulation, layered over\n"
        "the antiseptic that follows you through the whole complex.");
    set_items( ([
        "vats"     : "Upright growth tanks, lit from below. The shapes\n"
                     "inside are curled, canine, and not yet finished.",
        "tanks"    : "Two stories of them, shoulder to shoulder, each on\n"
                     "its own feed line and monitor loop.",
        "fluid"    : "Pale amber and faintly clouded. It is kept at body\n"
                     "temperature.",
        "cabling"  : "Feed lines and monitor cable in hanging bundles,\n"
                     "labeled by tank number.",
        "chalk"    : "A yield target chalked on the wall beside the shift\n"
                     "schedule. The number has been revised upward twice.",
        "schedule" : "A production shift schedule. Nothing on it reads any\n"
                     "differently than a schedule for making rifles."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Coalition scientist", this_object()))
        clone_object(NPC_PATH + "coalition_scientist.c")->move(this_object());
    if(!present("mutant subject", this_object()) && !random(3))
        clone_object(MON_PATH + "ls_mutant_subject.c")->move(this_object());
}
