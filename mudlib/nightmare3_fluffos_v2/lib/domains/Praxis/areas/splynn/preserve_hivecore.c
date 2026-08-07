// /d/Praxis/areas/splynn/preserve_hivecore.c
// Splynn Preserves hunting loop, R21: Hive Heart, the innermost chamber
// beyond Hive Depths.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Hive Heart");
    set_long(
        "The throat-like gallery opens into a wide, domed chamber, the\n"
        "true center of the hive. Every wall pulses in the same slow\n"
        "rhythm, and the egg clusters here are thicker, older, more\n"
        "carefully tended than anything in the outer spur.\n\n"
        "Something bigger than a warrior holds this chamber, and it does\n"
        "not tolerate anything reaching the eggs. The only way out is\n"
        "back up, toward the outer depths.");
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/preserve_hivedeep"
    ]) );
    set_listen("default",
        "A single massive heartbeat, slower and heavier than anything felt\n"
        "in the outer galleries, and the constant wet clicking of many\n"
        "mandibles.");
    set_smell("default",
        "Formic acid thick enough to sting the eyes, and the cloying reek\n"
        "of the oldest egg clusters.");
    set_items( ([
        "eggs"    : "The oldest, most carefully tended egg clusters in the\n"
                    "hive, thick along every wall of the domed chamber.",
        "chamber" : "A wide, domed chamber at the true center of the hive,\n"
                    "every surface alive with slow motion.",
        "walls"   : "Chitin walls pulsing in a single slow rhythm, warm\n"
                    "under your hand."
    ]) );
}

void reset() {
    ::reset();
    if(!present("kydian overlord", this_object()))
        clone_object(MONSTER_PATH+"kydian_overlord.c")->move(this_object());
    if(!present("xiticix warrior", this_object()))
        clone_object(MONSTER_PATH+"xiticix_warrior.c")->move(this_object());
}
