/* /domains/Praxis/pit.c */
#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_short("a rubble-choked pit");
    set_long(
        "A wide excavation sunk into the earth beneath Praxis, its walls cracked\n"
        "and partially caved in. Broken stone and compacted dirt fill the lower\n"
        "half. Whatever purpose this pit once served has been buried.");
    set_items(
        (["walls" : "Cracked earth and broken stone, partially caved in.",
          "rubble" : "Broken masonry and compacted dirt fill the lower half.",
          "pit"   : "A wide excavation with no obvious way back up."]) );
    set_property("light", 1);
    set_property("no teleport", 1);
}

void reset() {
    ::reset();
}
