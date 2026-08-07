// /domains/ChiTown/areas/cs_civilian_arms.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Chi-Town, Licensed Arms Shop");
    set_long(
        "Armored glass, weapon racks, and a permit notice bolted to the\n"
        "wall. Civilian sidearms and blades only. Military stock is handled\n"
        "at the CS armory.\n\n"
        "The shopping arcade is east.");
    set_exits( ([
        "east" : "/domains/ChiTown/areas/cs_shopping_arcade"
    ]) );
    set_listen("default", "A soft alarm tone when the door seals.");
    set_smell("default", "Gun oil and polished glass.");
    set_items( ([
        "notice" : "CIVILIAN PERMIT REQUIRED. Violators will be detained."
    ]) );
}

void reset() {
    ::reset();
    if(!present("arms clerk", this_object()))
        clone_object("/domains/ChiTown/npcs/cs_arms_clerk")->move(this_object());
}
