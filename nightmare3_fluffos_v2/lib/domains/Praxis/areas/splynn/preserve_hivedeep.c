// /d/Praxis/areas/splynn/preserve_hivedeep.c
// Splynn Preserves hunting loop, R15: Hive Depths, deeper into the Xiticix spur.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Hive Depths");
    set_long(
        "The gallery narrows and turns fully organic, chitin walls giving\n"
        "way to something closer to a throat than a tunnel. This deep the\n"
        "hive is warmer still, and the pulse in the walls is a heartbeat you\n"
        "can feel through your boots. Clusters of pale, leathery eggs are\n"
        "cemented into alcoves along the passage, tended and guarded.\n\n"
        "This is close to the hive's heart, and the warriors here do not\n"
        "give ground easily. The only way out is back west, toward the\n"
        "outer spur.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/preserve_hive"
    ]) );
    set_listen("default",
        "A deep, layered heartbeat pulsing through the walls, and the wet\n"
        "clicking of mandibles from more than one direction at once.");
    set_smell("default",
        "Overpowering formic acid and the sweetish, organic reek of the egg\n"
        "clusters cemented into the walls.");
    set_items( ([
        "eggs"     : "Clusters of pale, leathery eggs cemented into alcoves\n"
                     "along the passage. Disturbing them seems like a very bad\n"
                     "idea.",
        "throat"   : "A gallery that has narrowed into something closer to a\n"
                     "living throat than a tunnel. It is warm on every side.",
        "heartbeat": "A slow, massive pulse felt through the floor and walls\n"
                     "alike. Something enormous is close, and it is alive."
    ]) );
}

void reset() {
    ::reset();
    if(!present("xiticix warrior", this_object()))
        clone_object(MONSTER_PATH+"xiticix_warrior.c")->move(this_object());
    if(random(10) < 4 && !present("splugorth minion", this_object()))
        clone_object(MONSTER_PATH+"splugorth_minion.c")->move(this_object());
    if(random(100) < 10 && !present("kydian overlord", this_object()))
        clone_object(MONSTER_PATH+"kydian_overlord.c")->move(this_object());
}
