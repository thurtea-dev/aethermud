/* /domains/newcamelot/areas/market_street1.c
   New Camelot - Market Street. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("New Camelot - Market Street");
    set_long(
        "The famous Market Street of New Camelot. Timber-framed shopfronts\n"
        "stand shoulder to shoulder with stalls of Euro-tech and magecraft.\n"
        "Banners of the white dragon snap overhead. The Blacksmith's Forge\n"
        "is north, its chimney streaming sparks. The street continues east.\n"
        "The main plaza is southwest.");
    set_listen("default",
        "Hammer blows from the forge, hawkers calling prices, gulls.");
    set_smell("default",
        "Coal smoke, hot metal, and fresh bread.");
    set_items( ([
        "banners" : "White dragon banners of the Camelot Order.",
        "stalls" : "Stalls selling everything from turnips to targeting computers.",
        "shopfronts" : "Timber-framed buildings in the old style, rebuilt with modern MDC materials underneath."
    ]) );
    set_exits( ([
        "southwest" : "/domains/newcamelot/areas/camelot_square",
        "north" : "/domains/newcamelot/areas/blacksmith_forge",
        "east" : "/domains/newcamelot/areas/market_street2"
    ]) );
}
