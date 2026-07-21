/* /domains/NewCamelot/areas/castle_dungeon.c
   Camelot Castle - Dungeon. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Camelot Castle - Dungeon");
    set_long(
        "The dungeon beneath Camelot Castle, cleaner than most but still a\n"
        "dungeon. Cells with mega-damage bars line a torchlit corridor.\n"
        "Most stand empty; the Order does not take many prisoners, and\n"
        "keeps fewer. The stair back up to the courtyard is behind you.");
    set_listen("default",
        "Water dripping somewhere out of sight.");
    set_smell("default",
        "Damp stone and straw.");
    set_items( ([
        "cells" : "MDC bars, straw pallets, and a bucket. Humane, barely.",
        "torches" : "Real torches. The dungeon predates the generators."
    ]) );
    set_exits( ([
        "up" : "/domains/NewCamelot/areas/castle_courtyard"
    ]) );
}

void reset() {
    ::reset();
    if(!present("prisoner", this_object()))
        new("/domains/NewCamelot/npcs/dungeon_prisoner")->move(this_object());
}
