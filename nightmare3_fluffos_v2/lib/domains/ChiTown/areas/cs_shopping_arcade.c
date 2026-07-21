// /domains/ChiTown/areas/cs_shopping_arcade.c
// Fortified-city civilian shops (memories: many new shops).

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Chi-Town, Shopping Arcade");
    set_long(
        "A covered arcade of Coalition-licensed shops. Gray tile, CS seals\n"
        "on every storefront, credit scanners at each door. Citizens move\n"
        "quickly. Patrols move slower.\n\n"
        "Ration depot east. Arms shop west. The approved vendor market is\n"
        "north. The main boulevard is south. A residential block opens\n"
        "northeast.");
    set_exits( ([
        "south"     : "/domains/ChiTown/areas/chitown_boulevard",
        "north"     : "/domains/ChiTown/areas/chitown_market",
        "east"      : "/domains/ChiTown/areas/cs_ration_depot",
        "west"      : "/domains/ChiTown/areas/cs_civilian_arms",
        "northeast" : "/domains/ChiTown/areas/cs_residential"
    ]) );
    set_listen("default",
        "Scanner beeps and bootsteps on tile.");
    set_smell("default",
        "Disinfectant and packaged food.");
    set_items( ([
        "storefronts" : "Licensed shops. Every sign carries a CS product seal.",
        "scanners" : "Credit and ID scanners. They log every purchase."
    ]) );
}

void reset() {
    ::reset();
    if(!present("cs grunt", this_object()))
        clone_object("/domains/ChiTown/npcs/cs_grunt")->move(this_object());
    if(!present("chi-town citizen", this_object()))
        clone_object("/domains/ChiTown/npcs/cs_citizen")->move(this_object());
}
