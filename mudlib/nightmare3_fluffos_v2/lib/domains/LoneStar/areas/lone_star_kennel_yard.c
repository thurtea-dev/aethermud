/* /domains/LoneStar/areas/lone_star_kennel_yard.c
   Lone Star Dog Boy kennel wing: the outdoor exercise yard.
   Hub of the kennel wing, east of the barracks block.
   Connects west to the barracks, north to the kennels, east to the
   training ring. */

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Lone Star: Kennel Yard");
    set_long(
        "A fenced dirt yard behind the barracks block, packed hard by years\n"
        "of running feet. Obstacle frames stand at one end: hurdles, a climb\n"
        "wall, a crawl pipe, all built at Dog Boy proportions rather than\n"
        "human ones. Water troughs line the fence, refilled on a timer.\n\n"
        "A whiteboard by the gate lists shift rotations in a blunt shorthand,\n"
        "each handler's name beside a string of unit numbers.\n\n"
        "The barracks block is west. The kennel rows are north, and the\n"
        "training ring lies east.");
    set_exits( ([
        "west"  : "/domains/LoneStar/areas/lone_star_barracks",
        "north" : "/domains/LoneStar/areas/lone_star_kennels",
        "east"  : "/domains/LoneStar/areas/lone_star_training_ring"
    ]) );
    set_listen("default",
        "Barking from the kennel rows north, answered and then cut short by\n"
        "a handler's whistle. Boots and claws on packed dirt.");
    set_smell("default",
        "Dust, wet fur, and the ammonia edge of a yard that gets hosed down\n"
        "rather than cleaned.");
    set_items( ([
        "obstacles"  : "Hurdles, a climb wall, and a crawl pipe, all sized\n"
                       "for a body shorter and heavier than a human's.",
        "frames"     : "Welded steel obstacle frames, scarred by claws at\n"
                       "every handhold.",
        "troughs"    : "Galvanized water troughs along the fence, refilled\n"
                       "on a timer whether anything is drinking or not.",
        "whiteboard" : "Shift rotations in blunt shorthand. Handlers get\n"
                       "names. The units they run get numbers.",
        "fence"      : "Chain link topped with outward-angled wire. It is\n"
                       "built to keep things in, not out.",
        "dirt"       : "Packed so hard by running feet that it takes a\n"
                       "print from nothing."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Dog Boy guard", this_object()))
        clone_object(NPC_PATH + "cs_dog_boy_guard.c")->move(this_object());
}
