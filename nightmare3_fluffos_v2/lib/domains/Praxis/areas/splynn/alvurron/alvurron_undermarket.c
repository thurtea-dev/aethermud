/* /domains/Praxis/areas/splynn/alvurron/alvurron_undermarket.c
   Alvurron, Undermarket. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, Undermarket");
    set_long(
        "A low vaulted cellar beneath the warrens where the real business\n"
        "of Alvurron is done. Goods here do not go on display: they are\n"
        "described, in whispers, and produced from iron strongboxes only\n"
        "when credits have already changed hands. A dealer works from a\n"
        "stone alcove at the back. The stair up leads to the warrens.");
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/alvurron/alvurron_warrens"
    ]) );
    set_listen("default",
        "Whispered negotiation and the click of dimensional locks.");
    set_smell("default", "Tallow smoke and cold iron.");
    set_items( ([
        "strongboxes" : "Iron boxes with dimensional locks. You could not open
one with a rail gun.",
        "alcove" : "The dealer's niche, lit by a single red witch-lamp.",
        "goods" : "You see nothing for sale. That is the point."
    ]) );
}

void reset() {
    ::reset();
    if(!present("dealer", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_dealer")->move(this_object());
}
