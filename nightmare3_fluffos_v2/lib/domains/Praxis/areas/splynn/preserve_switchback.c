// /d/Praxis/areas/splynn/preserve_switchback.c
// Splynn Preserves hunting loop, R12: Trail Switchback, a fork off the game trail.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Trail Switchback");
    set_long(
        "The narrower track climbs in a tight switchback, doubling back on\n"
        "itself twice to gain height over broken, rocky ground. The alien\n"
        "undergrowth thins up here, giving way to bare stone shouldering\n"
        "through the soil. Whatever uses this path does not mind the climb.\n\n"
        "The main game trail is back south, downhill. There is no other easy\n"
        "way from here except back the way you came.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/preserve_trail"
    ]) );
    set_listen("default",
        "Loose stone shifting somewhere above you on the switchback, and the\n"
        "click of claws finding purchase on bare rock.");
    set_smell("default",
        "Dust and dry stone, a change from the wet green smell of the trail\n"
        "below.");
    set_items( ([
        "switchback" : "A track doubling back on itself twice to climb the\n"
                       "slope. Steep, but walkable.",
        "stone"      : "Bare rock shouldering up through thinning soil, worn\n"
                       "smooth in places by regular traffic.",
        "climb"      : "The path upward is exposed. Anything below can see you\n"
                       "coming, and anything above can see you better."
    ]) );
}

void reset() {
    ::reset();
    if(random(10) < 5 && !present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
}
