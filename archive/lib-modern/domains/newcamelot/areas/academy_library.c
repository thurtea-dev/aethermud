/* /domains/newcamelot/areas/academy_library.c
   Royal Academy - Library. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Royal Academy - Library");
    set_long(
        "The academy library, three storeys of shelves joined by ladders\n"
        "on brass rails. Grimoires mutter to each other on the restricted\n"
        "balcony, held shut by copper bands. A reading table by the window\n"
        "holds a heavy tome left open on its chain. The quad is northwest.");
    set_listen("default",
        "Pages turning. Some of them by themselves.");
    set_smell("default",
        "Old paper, leather, and candle smoke.");
    set_items( ([
        "shelves" : "Everything from herbals to bound dissertations on rift mechanics.",
        "grimoires" : "Chained and banded on the balcony. One of them is humming.",
        "table" : "Oak, with a chained spell tome open upon it."
    ]) );
    set_exits( ([
        "northwest" : "/domains/newcamelot/areas/academy_quad"
    ]) );
}

void reset() {
    ::reset();
    if(!present("spell tome", this_object()))
        new("/domains/newcamelot/equipment/spell_tome")->move(this_object());
}
