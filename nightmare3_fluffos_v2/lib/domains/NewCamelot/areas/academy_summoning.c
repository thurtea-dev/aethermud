/* /domains/NewCamelot/areas/academy_summoning.c
   Royal Academy - Summoning Chamber. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Royal Academy - Summoning Chamber");
    set_long(
        "A circular stone chamber deep beneath the quad. A triple ring of\n"
        "silver, salt and cold iron is set into the floor, and the walls\n"
        "are dense with warding sigils. Something small and indignant is\n"
        "usually bound in the circle, awaiting the next practical exam.\n"
        "The stair up leads back to the quad.");
    set_listen("default",
        "A low, continuous grumble from the circle.");
    set_smell("default",
        "Salt, scorched air, and brimstone.");
    set_items( ([
        "circle" : "Silver, salt, and cold iron. Triple redundancy is the first thing they teach.",
        "sigils" : "Layer on layer, some glowing faintly."
    ]) );
    set_exits( ([
        "up" : "/domains/NewCamelot/areas/academy_quad"
    ]) );
}

void reset() {
    ::reset();
    if(!present("lesser fire elemental", this_object()))
        new("/domains/NewCamelot/npcs/lesser_fire_elemental")->move(this_object());
}
