// /d/Praxis/areas/rockys_bar.c
// Rocky's Bar: rest spot west of Monument Square.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Rocky's Bar");
    set_long(
        "A long narrow room that smells of synthetic spirits and old smoke.\n"
        "The bar itself is a solid slab of pre-Rifts formica held up by\n"
        "welded rebar legs. Mismatched stools and salvaged tables fill\n"
        "the space. A hand-painted mural on the back wall shows a Rift\n"
        "tearing open over a burning city. Somebody's idea of decor.\n\n"
        "Rocky tends bar at one end, polishing a glass with a rag that\n"
        "has seen better days. Drinks are 50 credits. Say 'rumor' if\n"
        "you want to know what's moving on the street.");
    set_exits( (["east" : "/domains/Praxis/areas/monument_square"]) );
    set_listen("default",
        "Low conversation, the clink of glasses, and the buzz of a neon sign running on backup power.");
    set_smell("default",
        "Synthetic alcohol, stale smoke, and the faint electrical tang of the neon.");
    set_items( ([
        "bar" : "A solid slab of pre-Rifts formica. Rocky stands behind it.",
        "mural" : "A painted Rift tears open over a burning skyline. Cheerful.",
        "stools" : "Mismatched bar stools. Several are bolted down.",
        "sign" : "A neon sign reads ROCKY'S in red. It flickers occasionally.",
        "tables" : "Salvaged tables and chairs. Sturdy enough.",
        "neon" : "The neon is handmade: coiled glass tubing and a jury-rigged power supply."
    ]) );
}

void reset() {
    ::reset();
    if(!present("rocky", this_object()))
        clone_object(MONSTER_PATH+"rocky_barkeep.c")->move(this_object());
}
