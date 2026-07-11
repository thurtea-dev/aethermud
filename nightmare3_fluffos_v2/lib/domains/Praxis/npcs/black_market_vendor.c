// /d/Praxis/npcs/black_market_vendor.c
// Black market vendor in Splynn's hidden chamber.
// Sells predator armor and net gun for credits. No names used here.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/splynn/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("vendor");
    set_npc_name("vendor");
    set_id( ({ "vendor", "merchant", "seller", "figure", "contact", "man", "woman" }) );
    set_short("a shadowed vendor");
    set_long(
        "A compact figure seated behind a folding table, dressed in\n"
        "dark clothing with no insignia and no visible weapons, though\n"
        "you strongly suspect both conclusions are wrong. The face is\n"
        "partially obscured by a hood. The eyes that watch you from\n"
        "the shadow are calm, professional, and measuring.\n\n"
        "This person runs the black market in Splynn. They have not\n"
        "introduced themselves and will not. Type 'list' to see stock.\n"
        "Type 'buy <item>' to purchase.");
    set_level(8);
    set_max_hp(150);
    set_hp(150);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(-200);
    set_body_type("human");

    add_response("hello", "The vendor nods once and waits.");
    add_response("hi", "The vendor nods once and waits.");
    add_response("name", "The vendor says: 'Names are bad for business.'");
    add_response("who", "The vendor says: 'Irrelevant. What do you need?'");
    add_response("stock", "");
    add_response("inventory", "");
    add_response("help", "The vendor says: 'Type list to see what is available. Buy item name to purchase. Black market credits only. Smugglers can exchange credits with the exchange command.'");
    add_response("price", "");
    add_response("cost", "");
    add_response("bye", "The vendor says: 'Tell no one you were here.'");
    set_default_response("The vendor watches you and says nothing.");
    set_property("position_str", "stands in the shadows, watching.");

    set_shop_sells(0);
    set_shop_currency("black credits");
    set_shop_items(([
        "predator armor" : ({ EQ_PATH+"predator_armor.c", 3500, "predator armor" }),
        "predator" : ({ EQ_PATH+"predator_armor.c", 3500, "predator armor" }),
        "armor" : ({ EQ_PATH+"predator_armor.c", 3500, "predator armor" }),
        "net gun" : ({ EQ_PATH+"net_gun.c", 800, "net gun" }),
        "net" : ({ EQ_PATH+"net_gun.c", 800, "net gun" }),
        "plasma pistol" : ({ EQ_PATH+"plasma_pistol.c", 25000, "plasma pistol" }),
        "plasma" : ({ EQ_PATH+"plasma_pistol.c", 25000, "plasma pistol" })
    ]));
}

void catch_tell(string str) {
    string a, b;

    if(sscanf(str, "%s says: %s", a, b) != 2) {
        ::catch_tell(str);
        return;
    }
    b = lower_case(b);
    if(strsrch(b, "list") != -1 || strsrch(b, "stock") != -1 ||
       strsrch(b, "inventory") != -1 || strsrch(b, "price") != -1 ||
       strsrch(b, "cost") != -1) {
        call_out("show_stock", 1);
        return;
    }
    ::catch_tell(str);
}

void show_stock() {
    tell_room(environment(this_object()),
        "The vendor slides a laminated card across the table:\n"
        " predator armor  MDC 80, optical camo     3,500 black credits\n"
        " net gun         entanglement, non-lethal   800 black credits\n"
        " plasma pistol   6d6 MD, 400ft range     25,000 black credits\n"
        "\nBlack market credits only. Smugglers: type 'exchange' to convert.\n"
        "Type: buy predator armor  /  buy net gun  /  buy plasma pistol\n",
        ({ this_object() }));
}

int shop_list(string str) {
    show_stock();
    return 1;
}
