/* /domains/wizards/lounge_sword_vendor.c
   Brand, blade specialist in the Wizard's Lounge.
   Sells swords and bladed weapons for wizard combat testing.
   Commands: list, buy <item> */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();
    set_name("Brand");
    set_npc_name("Brand");
    set_id( ({ "brand", "blade specialist", "sword vendor", "sword seller",
               "vendor", "merchant", "man" }) );
    set_short("Brand, blade specialist");
    set_long(
        "A lean man with a collection of scars on his forearms that tell the\n"
        "history of a career spent with bladed weapons. He keeps his display\n"
        "rack well maintained. The blades range from practical vibro-weapons\n"
        "to rarer MDC-rated magical arms.\n"
        "Type 'list' to see available blades. Type 'buy <item>' to purchase.\n"
    );
    set_level(5);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(50);
    set_body_type("human");
    set_property("position_str", "stands by the blade display rack.");

    add_response("hello", "Brand says: Looking for something with an edge? Check the list.");
    add_response("hi", "Brand says: Looking for something with an edge? Check the list.");
    add_response("list", "Brand says: Type 'list' to see what I carry.");
    add_response("buy", "Brand says: Type 'buy <item>' to purchase.");
    add_response("sell", "Brand says: I don't buy. Too much paperwork.");
    add_response("mdc", "Brand says: The obsidian blade, katana, and Sword of Atlantis are all MDC-rated.");
    add_response("sdc", "Brand says: The vibro-sword, vibro-knife, and short sword are SDC weapons.");
    add_response("bye", "Brand says: Happy testing.");
    add_response("help", "Brand says: 'list' shows stock. 'buy <name>' purchases. Ask about 'mdc' for MDC blades.");
    set_default_response("Brand adjusts a display blade and waits.");
    set_shop_sells(0);

    set_shop_items( ([
        "vibro-knife"   : ({ EQ_PATH+"vibro_knife.c",      2000, "vibro-knife" }),
        "vibro knife"   : ({ EQ_PATH+"vibro_knife.c",      2000, "vibro-knife" }),
        "knife"         : ({ EQ_PATH+"vibro_knife.c",      2000, "vibro-knife" }),
        "vibro-sword"   : ({ EQ_PATH+"vibro_sword.c",      5000, "vibro-sword" }),
        "vibro sword"   : ({ EQ_PATH+"vibro_sword.c",      5000, "vibro-sword" }),
        "short sword"   : ({ EQ_PATH+"short_sword.c",      1000, "short sword" }),
        "short"         : ({ EQ_PATH+"short_sword.c",      1000, "short sword" }),
        "obsidian blade": ({ EQ_PATH+"obsidian_blade.c",   4000, "obsidian blade (MDC)" }),
        "obsidian"      : ({ EQ_PATH+"obsidian_blade.c",   4000, "obsidian blade (MDC)" }),
        "ghostly katana": ({ EQ_PATH+"ghostly_katana.c",   9000, "ghostly katana (MDC)" }),
        "katana"        : ({ EQ_PATH+"ghostly_katana.c",   9000, "ghostly katana (MDC)" }),
        "sword of atlantis": ({ EQ_PATH+"sword_of_atlantis.c", 15000, "Sword of Atlantis (MDC)" }),
        "atlantis sword": ({ EQ_PATH+"sword_of_atlantis.c", 15000, "Sword of Atlantis (MDC)" })
    ]) );
}
