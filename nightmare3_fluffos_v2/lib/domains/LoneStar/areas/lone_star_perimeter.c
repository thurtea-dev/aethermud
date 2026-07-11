/* /domains/LoneStar/areas/lone_star_perimeter.c
   Lone Star complex outer perimeter road.
   The approach between the facility gate and the open road.
   D-Bee subjects occasionally escape the research wing and appear here.
   Connects south to the outer gate. */

#define MONSTER_PATH "/domains/Praxis/monsters/"
#define LS_MON_PATH  "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Outer Perimeter");
    set_long(
        "The perimeter road between the Lone Star complex and the open approach.\n"
        "Razor-wire fencing runs along the east side. A secondary wall parallels\n"
        "it ten meters in, with automated sensor arrays at fifty-meter intervals.\n"
        "Coalition signage warns against attempting entry without clearance.\n\n"
        "Out here the Coalition's control is thinner. Lone Star's research\n"
        "program occasionally produces subjects that escape the inner facility.\n"
        "Dog Boy patrols sweep the perimeter on a schedule, but gaps exist.\n"
        "What gets through the inner fence is something that used to be a\n"
        "D-Bee, altered and unpredictable.\n\n"
        "The outer gate is south.  A sealed corridor leads west to containment.");
    set_exits( ([
        "south" : "/domains/LoneStar/areas/lone_star_gate",
        "west"  : "/domains/LoneStar/areas/lone_star_containment"
    ]) );
    set_listen("default",
        "The wind across flat ground. A sensor array rotating. The occasional\n"
        "crack of something moving in the razor-wire, then stillness.");
    set_smell("default",
        "Dust, dry grass, and the faint ozone of the sensor arrays running.\n"
        "Something else underneath: chemical and biological, from the facility.");
    set_items( ([
        "fence"    : "Razor-wire fencing. Military grade. Coalition skull markers "
                     "every ten meters, lit amber at night.",
        "sensors"  : "Automated sensor arrays on posts. Motion, thermal, dimensional. "
                     "They see most things.",
        "wire"     : "Razor-wire. Something has pushed through it in one spot recently. "
                     "The wire has been bent outward.",
        "wall"     : "A secondary inner wall, ferroconcrete. Taller than the outer fence. "
                     "Things have scratched its face from the inside.",
        "signs"    : "LONE STAR GENETICS RESEARCH COMPLEX. RESTRICTED ZONE. "
                     "LETHAL FORCE AUTHORIZED.",
        "scratches": "Deep scratch marks on the inner wall surface. "
                     "Whatever made them had claws. Or did once."
    ]) );
}

void reset() {
    ::reset();
    if(!present("mutant subject", this_object()) && !random(3))
        clone_object(LS_MON_PATH + "ls_mutant_subject.c")->move(this_object());
    if(!present("D-Bee wanderer", this_object()) && !random(3))
        clone_object(MONSTER_PATH + "d_bee_wanderer.c")->move(this_object());
    if(!present("jungle stalker", this_object()) && !random(4))
        clone_object(MONSTER_PATH + "jungle_stalker.c")->move(this_object());
}
