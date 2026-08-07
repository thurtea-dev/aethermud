/* /domains/NGR/areas/NGR_market_district.c
   New German Republic - Market District.
   A busy commercial zone inside the gates. Clean, orderly, technically advanced. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("ngr_territory", 1);
    set_short("NGR Market District");
    set_long(
        "The market district of the New German Republic operates with the same\n"
        "precision as everything else here. Stalls are arranged in clean rows,\n"
        "each one assigned a number. No hawking. No haggling, prices are posted\n"
        "on electronic boards and they are fixed. Vendors wear identification\n"
        "badges. The whole district is surveilled by security cameras and\n"
        "patrolling NGR soldiers.\n\n"
        "The merchandise is impressive: NGR-manufactured weapons, power cells,\n"
        "military-grade electronics, and salvaged Coalition tech. Some stalls\n"
        "carry Triax equipment, the giant mecha manufacturer headquartered\n"
        "here in Germany. Prices are fair but not cheap. Everything is legal,\n"
        "documented, and taxed.\n\n"
        "The main gate is to the west. The Triax equipment depot is east.\n"
        "The garrison district lies south.");
    set_exits( ([
        "west"  : "/domains/NGR/areas/NGR_gate",
        "east"  : "/domains/NGR/areas/NGR_triax_depot",
        "south" : "/domains/NGR/areas/NGR_garrison",
        "north" : "/domains/NGR/areas/ngr_plaza"
    ]) );
    set_listen("default",
        "The methodical noise of commerce: transactions, equipment being tested,\n"
        "a soldier calling numbers for the queue. No shouting.");
    set_smell("default",
        "Metal, electronics, the faint smell of weapon oil, and something like\n"
        "fresh bread from a food vendor three rows over.");
    set_items( ([
        "stalls"  : "Each stall has a number and a price board. "
                    "Prices are fixed. Do not try to negotiate.",
        "weapons" : "NGR-manufactured energy weapons and conventional arms. "
                    "Documentation required for purchase. Visitors can buy, but "
                    "purchases are logged.",
        "triax"   : "A stall bearing the Triax logo. Heavy iron eagle on a red field. "
                    "The equipment here is the best humanity produces on this continent.",
        "cameras" : "Security cameras cover every angle of the market. "
                    "The NGR watches everything inside its territory.",
        "soldiers" : "NGR patrol soldiers moving through the market in pairs. "
                     "Grey armor, rail guns shouldered. Not aggressive -- just present."
    ]) );
}

void reset() { ::reset(); }
