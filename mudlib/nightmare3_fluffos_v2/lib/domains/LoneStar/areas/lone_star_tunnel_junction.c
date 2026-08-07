/* /domains/LoneStar/areas/lone_star_tunnel_junction.c
   Lone Star escape route: the junction past the torn grating.
   Where the maintenance run splits toward the sump and the surface.
   Connects south to the breached tunnel, west to the sump, north to the
   culvert mouth. */

#define MONSTER_PATH "/domains/Praxis/monsters/"
#define LS_MON_PATH  "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Tunnel Junction");
    set_long(
        "Past the torn grating the maintenance run opens into a junction\n"
        "chamber where three concrete bores meet under a rusted access\n"
        "ladder. The ladder's lower rungs are gone. Someone unbolted them\n"
        "rather than cut them, which took time and took tools.\n\n"
        "Chalk marks cover one wall at knee height: tally strokes, arrows,\n"
        "and a crude repeated symbol that is not any Coalition code. Whoever\n"
        "came through has been coming through for a while, and has been\n"
        "leaving directions for whoever comes next.\n\n"
        "The breached tunnel is back south. The west bore runs down toward\n"
        "standing water. The north bore climbs toward daylight.");
    set_exits( ([
        "south" : "/domains/LoneStar/areas/lone_star_escape_tunnel",
        "west"  : "/domains/LoneStar/areas/lone_star_sump",
        "north" : "/domains/LoneStar/areas/lone_star_culvert_mouth"
    ]) );
    set_listen("default",
        "Water moving somewhere west, and a hollow draft pulling north\n"
        "through the bores. No machinery. Nothing maintains this anymore.");
    set_smell("default",
        "Wet concrete, rust, and a thin chemical trace carried up from the\n"
        "complex on the draft.");
    set_items( ([
        "ladder"  : "A rusted access ladder to a sealed hatch overhead. The\n"
                    "lower rungs have been unbolted and taken away.",
        "chalk"   : "Tally strokes, arrows, and a repeated symbol at knee\n"
                    "height. Directions, left for someone else.",
        "marks"   : "Not Coalition marking. Whoever wrote them was working\n"
                    "low to the ground and in a hurry.",
        "symbol"  : "The same crude shape, drawn over and over. It means\n"
                    "something to whoever is using this route.",
        "bores"   : "Three concrete bores meeting under the ladder. West\n"
                    "runs down, north climbs, south goes back under the\n"
                    "fence line."
    ]) );
}

void reset() {
    ::reset();
    if(!present("mutant subject", this_object()) && !random(3))
        clone_object(LS_MON_PATH + "ls_mutant_subject.c")->move(this_object());
    if(!present("D-Bee wanderer", this_object()) && !random(4))
        clone_object(MONSTER_PATH + "d_bee_wanderer.c")->move(this_object());
}
