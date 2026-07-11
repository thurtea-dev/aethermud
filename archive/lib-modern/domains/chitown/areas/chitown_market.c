// /domains/chitown/areas/chitown_market.c
// Coalition-approved vendor district.

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/chitown/npcs/"
#define MOXIM_PATH "/domains/Praxis/monsters/moxim"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Chi-Town, Approved Vendor District");
    set_long(
        "The Coalition-approved commercial zone. Every stall is ordered,\n"
        "branded with CS markings, and staffed by licensed vendors with\n"
        "verified human ID. Prices are standardized. Goods are Coalition\n"
        "approved. Non-humans are not permitted on these premises.\n\n"
        "Military requisitions are handled at the CS armory west of the\n"
        "command plaza. Civilian stalls line both sides of the district.");
    set_exits( ([
        "south" : "/domains/chitown/areas/chitown_officer_row",
        "east" : "/domains/chitown/areas/chitown_burbs",
        "down" : "/domains/chitown/areas/chitown_lower_market"
    ]) );
    set_listen("default",
        "Transaction beeps, the click of credit scanners, and CS music.");
    set_smell("default",
        "New equipment, gun oil, and packaged rations.");
    set_items( ([
        "depot" : "Licensed vendor stalls. Coalition-approved consumer goods only.",
        "stalls" : "Orderly stalls with CS markings on everything.",
        "signs" : "Coalition product seals on every item. Nothing unapproved here."
    ]) );
}

void reset() {
    ::reset();
    if(!present("moxim", this_object()))
        clone_object(MOXIM_PATH)->move(this_object());
    if(!present("chi-town citizen", this_object()))
        clone_object(NPC_PATH+"cs_citizen")->move(this_object());
}
