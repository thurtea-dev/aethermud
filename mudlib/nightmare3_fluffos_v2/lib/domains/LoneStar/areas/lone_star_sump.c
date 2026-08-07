/* /domains/LoneStar/areas/lone_star_sump.c
   Lone Star escape route: the flooded sump below the junction.
   Dead end holding what escapees dropped on the way out.
   Connects east to the tunnel junction. */

#define EQ_PATH      "/domains/Praxis/equipment/"
#define LS_MON_PATH  "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Flooded Sump");
    set_long(
        "The west bore ends in a sump pit, knee deep in cold standing water\n"
        "that has not drained since the pumps were cut off the grid. The\n"
        "walls are furred with pale growth to the old waterline, well above\n"
        "where the water sits now.\n\n"
        "Things have been thrown down here rather than carried out: a\n"
        "specimen sleeve, a cracked monitor cuff, a boot with no partner.\n"
        "Anyone running for the surface lightens their load first, and this\n"
        "is the last place to do it out of sight.\n\n"
        "The junction is back east. There is no other way out of the pit.");
    set_exits( ([
        "east" : "/domains/LoneStar/areas/lone_star_tunnel_junction"
    ]) );
    set_listen("default",
        "Your own movement in the water, doubled and returned by the pit\n"
        "walls. Between movements, dripping, and nothing else.");
    set_smell("default",
        "Stagnant water, wet rot, and the antiseptic that everything\n"
        "leaving the complex carries out with it.");
    set_items( ([
        "water"     : "Knee deep, cold, and still. The pumps stopped a long\n"
                      "time ago.",
        "growth"    : "Pale furred growth up the walls to an old waterline,\n"
                      "well above the present level.",
        "waterline" : "A stain far above the water. This pit used to be\n"
                      "full.",
        "sleeve"    : "A specimen sleeve, torn open, its line code tag\n"
                      "still legible under the silt.",
        "cuff"      : "A cracked monitor cuff, the kind locked onto a\n"
                      "subject's wrist. It was broken off, not unlocked.",
        "boot"      : "One boot, Coalition issue, sized for a foot that is\n"
                      "not shaped like a human's."
    ]) );
}

void reset() {
    ::reset();
    if(!present("salvage", this_object()) && !random(3))
        clone_object(EQ_PATH + "salvage_item.c")->move(this_object());
    if(!present("mutant subject", this_object()) && !random(2))
        clone_object(LS_MON_PATH + "ls_mutant_subject.c")->move(this_object());
}
