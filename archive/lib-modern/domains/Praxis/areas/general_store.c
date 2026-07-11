// /d/Praxis/areas/general_store.c
// Marta's general goods store off Market Street.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Marta's General Goods");
    set_long(
        "A cramped but well-organized stall that spills onto the street.\n"
        "Wire crates and salvaged shelving hold an unlikely mix of gear:\n"
        "medical supplies, food packs, rope, tools, and the occasional\n"
        "piece of clothing. Everything has been sorted and priced in\n"
        "a neat hand. Type 'list' to see the stock,\n"
        "or 'buy <item>' to purchase. Credits only.");
    set_exits( (["north" : "/domains/Praxis/areas/market_street"]) );
    set_listen("default",
        "Market noise drifting in, the scratch of pen on paper as Marta keeps inventory.");
    set_smell("default",
        "Antiseptic, vacuum-sealed food, and canvas.");
    set_items( ([
        "shelving" : "Salvaged metal shelving loaded with survival supplies.",
        "crates" : "Wire crates labeled in neat writing: MEDS, FOOD, MISC.",
        "counter" : "Marta's workspace. Neat, efficient, everything in reach.",
        "price tags": "Small hand-written cards on each item. All prices in credits."
    ]) );
}

void reset() {
    ::reset();
    if(!present("marta", this_object()))
        clone_object(MONSTER_PATH+"general_vendor.c")->move(this_object());
}
