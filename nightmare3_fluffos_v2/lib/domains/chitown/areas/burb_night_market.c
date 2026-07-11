// /domains/chitown/areas/burb_night_market.c
// Always-open street market (MUD has no day/night cycle requirement).

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("Burb Night Market");
    set_long(
        "Tarps and battery lamps turn a side street into a market that never\n"
        "quite closes. Vendors sell scrap, food, and things best not named\n"
        "near a Dog Boy. Deals happen in murmurs.\n\n"
        "The chop shop is west. South leads to deep burb row. A fence stall\n"
        "huddles under a lean-to to the east.");
    set_exits( ([
        "west"  : "/domains/chitown/areas/burb_chop_shop",
        "south" : "/domains/chitown/areas/chitown_burb_row",
        "east"  : "/domains/chitown/areas/burb_fence_stall"
    ]) );
    set_listen("default",
        "Haggling, a generator coughing, boots on wet boards.");
    set_smell("default",
        "Fried something, lamp oil, and damp canvas.");
    set_items( ([
        "lamps" : "Battery lamps hung from wire. Some flicker.",
        "tarps" : "Sun-bleached and patched. They hide as much as they shelter.",
        "vendors" : "Faces change. The stalls stay."
    ]) );
}
