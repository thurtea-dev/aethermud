// /d/Praxis/areas/splynn/preserve_waterhole.c
// Splynn Preserves hunting loop, R3: the water hole, a killing ground.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Sunken Water Hole");
    set_long(
        "The ground sinks to a fetid pool ringed with churned mud and the\n"
        "flattened reeds where heavy bodies come to drink. The water is dark\n"
        "and still and smells of things that died in it. Every creature in\n"
        "the Preserves needs water, so every predator in the Preserves knows\n"
        "to wait here.\n\n"
        "Bones show pale at the waterline, gnawed and abandoned. The brake\n"
        "climbs away north, the ground falls south toward an open field of\n"
        "them, and thick reed cover crowds the bank to the east.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/preserve_thornbrake",
        "south" : "/domains/Praxis/areas/splynn/preserve_bonefield",
        "east"  : "/domains/Praxis/areas/splynn/preserve_reeds"
    ]) );
    set_listen("default",
        "The slow drip of water, the buzz of insects that are too large,\n"
        "and a heavy body shifting its weight in the reeds across the pool.");
    set_smell("default",
        "Stagnant water, wet rot, and the iron tang of a kill made recently\n"
        "at the edge of the mud.");
    set_items( ([
        "pool"  : "Dark, still water fouled by everything that has drunk and\n"
                  "died here. You would not want to wade it.",
        "reeds" : "Tall reeds flattened into beds where large animals have\n"
                  "lain in wait. The cover here works both ways.",
        "bones" : "Pale gnawed bones half-sunk in the mud at the waterline.\n"
                  "Some are recent. Some are shaped wrong for any Earth animal."
    ]) );
}

void reset() {
    ::reset();
    if(!present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
    if(random(10) < 3 && !present("xiticix warrior", this_object()))
        clone_object(MONSTER_PATH+"xiticix_warrior.c")->move(this_object());
}
