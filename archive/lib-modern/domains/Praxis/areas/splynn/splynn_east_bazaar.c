// /domains/Praxis/areas/splynn/splynn_east_bazaar.c
// Eastern bazaar stalls off the Dimensional Market.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 1);
    set_short("East Bazaar, Splynn");
    set_long(
        "A dense cluster of stalls east of the main market plaza. Alien\n"
        "merchants hawk weapons, bio-mod kits, and slaves with equal\n"
        "indifference. Kittani overseers watch from a raised platform.\n"
        "Chains clink. Dimensional protected makes your teeth ache.\n\n"
        "The Dimensional Market lies west. A narrow guild row opens south.");
    set_exits( ([
        "west"  : "/domains/Praxis/areas/splynn/splynn_market",
        "south" : "/domains/Praxis/areas/splynn/splynn_guild_row"
    ]) );
    set_listen("default",
        "Haggling in a dozen languages, clanking chains, and vendor calls.");
    set_smell("default",
        "Exotic spices, ozone, and blood.");
}

void reset() {
    ::reset();
    if(!present("kittani merchant", this_object()))
        clone_object("/domains/Praxis/monsters/kittani_merchant.c")->move(this_object());
}
