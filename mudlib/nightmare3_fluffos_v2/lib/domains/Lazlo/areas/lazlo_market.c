// /domains/Lazlo/areas/lazlo_market.c
// The market quarter of Lazlo -- open trade, spell components, rare salvage.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Lazlo, Market Quarter");
    set_long(
        "The commercial heart of Lazlo. Wide boulevards lined with permanent\n"
        "stone stalls, each with a sign in multiple languages. The goods here\n"
        "are different from Praxis: alongside salvaged tech and military surplus\n"
        "you'll find spell components, enchanted items, rare herbs, and\n"
        "dimensional trading goods from half a dozen planes of existence.\n\n"
        "The diversity of the shoppers matches the goods. Humans bargain\n"
        "alongside Elves, D-Bees, and things with no common name. A Lyn-Srial\n"
        "floats three feet off the ground while examining a crate of arcane\n"
        "components. Nobody stares.\n\n"
        "The arrival courtyard is south. The eastern gate approach is south-east.\n"
        "A sign points north toward the Great Library.");
    set_exits( ([
        "south" : "/domains/Lazlo/lazlo_welcome",
        "east" : "/domains/Lazlo/areas/lazlo_gate",
        "north" : "/domains/Lazlo/areas/lazlo_library",
        "west" : "/domains/Lazlo/areas/lazlo_square"
    ]) );
    set_listen("default",
        "Market noise in a dozen languages. Someone is haggling loudly in what might be Dragonese.");
    set_smell("default",
        "Exotic herbs, hot food, ozone from magical items, and the underlaying clean smell of the lake.");
    set_items( ([
        "stalls" : "Stone market stalls, permanent and well-built. Signs in multiple scripts.",
        "signs" : "Market signs in Common, Dragonese, and at least two other scripts.",
        "goods" : "Spell components, enchanted items, salvaged tech, dimensional trade goods.",
        "components" : "Herbs, crystals, powdered MDC bone, and things you don't have names for.",
        "lyn-srial" : "A winged alien being, floating while examining arcane components. It notices you noticing it and nods.",
        "crowd" : "Humans, Elves, D-Bees. More species here than in a Coalition nightmare."
    ]) );
}

void reset() {
    ::reset();
    if(!present("moxim", this_object()))
        clone_object("/domains/Praxis/monsters/moxim")->move(this_object());
}
