/* /domains/NGR/areas/NGR_triax_depot.c
   New German Republic -- Triax equipment depot, east of the market district.
   The official Triax TX-series vendor for authorized purchasers.
   Connects west to the market district. */

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/NGR/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("ngr_territory", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Triax Equipment Depot");
    set_long(
        "The official Triax Industries depot in the NGR market district. The\n"
        "interior is clean, brightly lit, and orderly in a way that clearly\n"
        "reflects the manufacturer's values. Display cases line both walls:\n"
        "energy weapons, power cells, communications gear, and field equipment\n"
        "arranged with precision. Every item has a specification card and a\n"
        "price. No variation, no bargaining.\n\n"
        "A Triax sales officer manages the counter: uniformed, efficient, and\n"
        "completely willing to sell you anything in the case, provided your\n"
        "papers are in order and your credits are genuine.\n\n"
        "The NGR market district is to the west.");
    set_exits( ([
        "west" : "/domains/NGR/areas/NGR_market_district"
    ]) );
    set_listen("default",
        "The quiet hum of display case climate control. A transaction being\n"
        "processed. The crisp sound of a receipt printer.");
    set_smell("default",
        "New equipment: the smell of machine oil, polymer, and sealed packaging.");
    set_items( ([
        "cases"    : "Display cases showing TX-series energy weapons, power cells, "
                     "and communications gear. Each item has specification data.",
        "weapons"  : "TX-series Triax energy weapons. Reliable, expensive, documented. "
                     "The NGR requires all purchases to be registered.",
        "officer"  : "The Triax sales officer. Uniformed, correct, ready to assist "
                     "if your documentation is valid.",
        "counter"  : "A clean polymer counter. The transaction terminal is flush-mounted. "
                     "Everything here is built to last.",
        "display"  : "Equipment displayed against white backing. Tactical gear, "
                     "energy cells, sensors, personal defense items.",
        "logo"     : "The Triax Industries logo above the counter: an iron eagle on a red field. "
                     "NGR's biggest industrial partner, and proud of it.",
        "prices"   : "Fixed. Posted on specification cards. Exactly what the card says, "
                     "not one credit more or less."
    ]) );
}

void reset() {
    ::reset();
    if(!present("triax sales officer", this_object()))
        clone_object(NPC_PATH + "ngr_arms_dealer.c")->move(this_object());
}
