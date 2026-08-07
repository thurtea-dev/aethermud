/* /domains/LoneStar/areas/lone_star_motorpool.c
   Lone Star vehicle bay, off the supply depot.
   Connects west to supply, northeast to the perimeter road. */

#define MONSTER_PATH "/domains/Praxis/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("vehicle_accessible", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Motor Pool");
    set_long(
        "A covered bay behind the supply depot, fuel drums stacked along\n"
        "one wall and a row of Coalition transports parked nose-out for a\n"
        "quick departure. A big roll-up door on the far side stands raised,\n"
        "opening onto the perimeter road northeast.\n\n"
        "The supply depot is back west.");
    set_exits( ([
        "west"      : "/domains/LoneStar/areas/lone_star_supply",
        "northeast" : "/domains/LoneStar/areas/lone_star_perimeter"
    ]) );
    set_listen("default",
        "The idle tick of an engine block cooling. Fuel dripping somewhere,\n"
        "slow and steady.");
    set_smell("default",
        "Diesel, hot metal, and rubber.");
    set_items( ([
        "transports" : "Coalition transports parked nose-out, fueled and\n"
                       "ready. Someone here plans for a fast exit.",
        "drums"      : "Stacked fuel drums, Coalition-stenciled, chained\n"
                       "together against the wall.",
        "door"       : "A big roll-up door on the far wall, raised. It\n"
                       "opens onto the perimeter road northeast."
    ]) );
}

void reset() {
    ::reset();
    if(random(4) == 0 && !present("D-Bee wanderer", this_object()))
        clone_object(MONSTER_PATH + "d_bee_wanderer.c")->move(this_object());
}
