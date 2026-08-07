/* /domains/LoneStar/areas/lone_star_culvert_mouth.c
   Lone Star escape route: where the drainage run surfaces outside the wire.
   Closes the escape loop back to the outer gate approach.
   Connects south to the tunnel junction, southeast to the outer gate. */

#define MONSTER_PATH "/domains/Praxis/monsters/"
#define LS_MON_PATH  "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Lone Star: Culvert Mouth");
    set_long(
        "The north bore surfaces in a shallow wash of scrub and broken\n"
        "concrete, well outside the wire. The culvert mouth is half hidden\n"
        "behind a stand of dead brush that did not grow there; it was cut\n"
        "and dragged into place, and it is replaced whenever it dries out.\n\n"
        "From here the complex is a low gray line on the horizon with two\n"
        "guard towers over it. Nothing in the sensor arrays faces this\n"
        "direction. The Coalition mapped the drainage as a hazard, not as an\n"
        "exit, and never revised the assessment.\n\n"
        "The culvert runs back south into the dark. The road up to the outer\n"
        "gate is southeast across open ground.");
    set_exits( ([
        "south"     : "/domains/LoneStar/areas/lone_star_tunnel_junction",
        "southeast" : "/domains/LoneStar/areas/lone_star_gate"
    ]) );
    set_listen("default",
        "Wind through dry scrub, and the draft moving in the culvert behind\n"
        "you, which never stops.");
    set_smell("default",
        "Dust and dry brush, with the wet concrete smell of the culvert at\n"
        "your back.");
    set_items( ([
        "culvert" : "A concrete mouth in the side of the wash, half hidden\n"
                    "and easy to walk past.",
        "brush"   : "Cut brush dragged into place across the opening.\n"
                    "Someone maintains it.",
        "wash"    : "A shallow drainage cut full of scrub and broken\n"
                    "concrete, below the line of sight from the towers.",
        "complex" : "A low gray line on the horizon, two guard towers over\n"
                    "it. Far enough that nothing there is looking at you.",
        "towers"  : "Facing the road and the fence line. Not facing the\n"
                    "drainage.",
        "ground"  : "Open scrub between the wash and the gate road. No\n"
                    "cover at all across it."
    ]) );
}

void reset() {
    ::reset();
    if(!present("mutant subject", this_object()) && !random(3))
        clone_object(LS_MON_PATH + "ls_mutant_subject.c")->move(this_object());
    if(!present("D-Bee wanderer", this_object()) && !random(4))
        clone_object(MONSTER_PATH + "d_bee_wanderer.c")->move(this_object());
}
