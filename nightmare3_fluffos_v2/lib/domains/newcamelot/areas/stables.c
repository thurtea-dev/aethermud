/* /domains/newcamelot/areas/stables.c
   Royal Stables. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Royal Stables");
    set_long(
        "The royal stables of New Camelot. Two rows of stalls face each\n"
        "other across a swept brick aisle. Some stalls hold horses; others\n"
        "hold things that are almost horses, with too many joints or a\n"
        "faint shimmer of scales. Tack and barding hang from every post.\n"
        "Market Street is south.");
    set_listen("default",
        "Stamping hooves and the slow grind of hay being chewed.");
    set_smell("default",
        "Hay, leather, and warm animal.");
    set_items( ([
        "stalls" : "Solid oak reinforced with steel. Some of the occupants are strong.",
        "horses" : "Warhorses, riding hacks, and a few dimension-touched beasts only a Dragon Knight would saddle.",
        "barding" : "MDC plate barding, white dragon on the chamfron."
    ]) );
    set_exits( ([
        "south" : "/domains/newcamelot/areas/market_street2"
    ]) );
}

void reset() {
    ::reset();
    if(!present("stable hand", this_object()))
        new("/domains/newcamelot/npcs/stable_hand")->move(this_object());
}
