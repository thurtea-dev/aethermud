// /domains/chitown/areas/cs_ration_depot.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Chi-Town, Ration Depot");
    set_long(
        "Steel shelves of sealed meal packs. A counter with a scanner and\n"
        "a queue line painted on the floor. No haggling. No substitutions.\n\n"
        "The shopping arcade is west. An outfitter shop is north.");
    set_exits( ([
        "west"  : "/domains/chitown/areas/cs_shopping_arcade",
        "north" : "/domains/chitown/areas/cs_outfitter_shop"
    ]) );
    set_listen("default", "A scanner chirp. Quiet waiting.");
    set_smell("default", "Plastic wrap and dried protein.");
}

void reset() {
    ::reset();
    if(!present("ration clerk", this_object()))
        clone_object("/domains/chitown/npcs/cs_ration_clerk")->move(this_object());
}
