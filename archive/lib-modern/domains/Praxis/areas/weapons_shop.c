// /d/Praxis/areas/weapons_shop.c
// Griz Arms: weapons shop off Market Street.

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
    set_short("Griz Arms, weapons shop");
    set_long(
        "A reinforced storeroom converted into a weapons shop. The walls\n"
        "are lined with pegboard hung with holsters, slings, and maintenance\n"
        "kits. A heavy-gauge security cage in the back holds the more serious\n"
        "merchandise. The workbench in the center is scarred by years of\n"
        "disassembly and repair. Type 'list' to see the stock,\n"
        "or 'buy <item>' to purchase. Credits only.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/market_street",
        "east" : "/domains/Praxis/areas/long_road"
    ]) );
    set_listen("default",
        "The hum of charging equipment and the metallic click of Griz working.");
    set_smell("default",
        "Gun oil, solder, and the faint burnt-ozone of energy weapon residue.");
    set_items( ([
        "cage" : "The security cage is locked. The really expensive stuff is in there.",
        "workbench" : "Scarred steel topped with parts, tools, and cleaning supplies.",
        "pegboard" : "Holsters, mag pouches, slings. Accessories for the discerning survivor.",
        "counter" : "Heavy blast-rated glass over a display of sidearms."
    ]) );
}

void reset() {
    ::reset();
    if(!present("griz", this_object()))
        clone_object(MONSTER_PATH+"weapons_vendor.c")->move(this_object());
}
