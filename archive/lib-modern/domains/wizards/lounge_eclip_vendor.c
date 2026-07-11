/* /domains/wizards/lounge_eclip_vendor.c
   Sparks, tech specialist in the Wizard's Lounge.
   Sells e-clips and recharges depleted ones.
   Commands: list, buy <item>, recharge <name|all>
   Wizard staff (creatorp) pay no recharge fee. */

#include <std.h>
#include <daemons.h>

#define EQ_PATH  "/domains/Praxis/equipment/"
#define AMO_PATH "/domains/Praxis/equipment/ammo/"

inherit "/std/rifts_vendor";

void create() {
    ::create();
    set_name("Sparks");
    set_npc_name("Sparks");
    set_id( ({ "sparks", "tech specialist", "clip vendor", "eclip vendor",
               "recharger", "specialist", "man" }) );
    set_short("Sparks, tech specialist");
    set_long(
        "A young man with permanently singed eyebrows and quick, precise\n"
        "hands. A charging rack behind him holds a dozen e-clips in various\n"
        "states of charge, each with a small indicator LED. He sells clips\n"
        "and will recharge depleted ones on the spot.\n"
        "Type 'list' for stock. 'buy <item>' to purchase.\n"
        "'recharge all' to restore all depleted e-clips you carry.\n"
    );
    set_level(3);
    set_max_hp(50);
    set_hp(50);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(100);
    set_body_type("human");
    set_property("position_str", "tends the charging rack.");

    add_response("hello", "Sparks says: Need clips or a recharge? I've got you covered.");
    add_response("hi", "Sparks says: Need clips or a recharge? I've got you covered.");
    add_response("list", "Sparks says: Type 'list' to see my stock.");
    add_response("buy", "Sparks says: Type 'buy <item>' to purchase.");
    add_response("recharge", "Sparks says: Type 'recharge all' to top off all your clips. 50 credits each.");
    add_response("charge", "Sparks says: Type 'recharge all' or 'recharge <clip name>'.");
    add_response("sell", "Sparks says: I don't buy. Try the black market.");
    add_response("bye", "Sparks says: Keep your clips topped off.");
    add_response("help", "Sparks says: 'list' shows stock. 'buy <name>' buys. 'recharge all' recharges.");
    set_default_response("Sparks checks a clip indicator and notes the reading.");
    set_shop_sells(0);

    set_shop_items( ([
        "e-clip"         : ({ EQ_PATH+"e_clip.c",           300, "standard e-clip (30 shots)" }),
        "eclip"          : ({ EQ_PATH+"e_clip.c",           300, "standard e-clip (30 shots)" }),
        "clip"           : ({ EQ_PATH+"e_clip.c",           300, "standard e-clip (30 shots)" }),
        "long e-clip"    : ({ AMO_PATH+"e_clip_long.c",     500, "long e-clip (60 shots)" }),
        "long clip"      : ({ AMO_PATH+"e_clip_long.c",     500, "long e-clip (60 shots)" }),
        "standard e-clip": ({ AMO_PATH+"e_clip_standard.c", 300, "standard e-clip" }),
        "standard clip"  : ({ AMO_PATH+"e_clip_standard.c", 300, "standard e-clip" })
    ]) );
}

void init() {
    ::init();
    add_action("cmd_recharge", "recharge");
}

int cmd_recharge(string str) {
    object *inv;
    object ob;
    int i;
    int count;
    int total_cost;
    int max_ammo;
    int cur_ammo;
    int is_wiz;

    if(!str || !sizeof(str)) {
        write("Recharge what? Try 'recharge all' or 'recharge <clip name>'.\n");
        return 1;
    }
    str = lower_case(str);
    is_wiz = creatorp(this_player());
    inv = all_inventory(this_player());

    /* First pass: count depleted clips matching request */
    count = 0;
    for(i = 0; i < sizeof(inv); i++) {
        ob = inv[i];
        if(!ob || !objectp(ob)) continue;
        if(!(int)ob->query_property("e_clip")) continue;
        if(str != "all" && !(int)ob->id(str)) continue;
        max_ammo = (int)ob->query_property("ammo_max");
        cur_ammo = (int)ob->query_property("ammo_count");
        if(max_ammo <= 0) max_ammo = 30;
        if(cur_ammo < max_ammo) count++;
    }

    if(!count) {
        say_here("No depleted e-clips on you match that.");
        return 1;
    }

    total_cost = count * 50;
    if(!is_wiz && (int)this_player()->query_money("credits") < total_cost) {
        say_here("Recharging " + count + " clip" +
            (count == 1 ? "" : "s") + " costs " + total_cost +
            " credits. You're " + (total_cost - (int)this_player()->query_money("credits")) +
            " short.");
        return 1;
    }

    /* Second pass: apply recharge */
    count = 0;
    for(i = 0; i < sizeof(inv); i++) {
        ob = inv[i];
        if(!ob || !objectp(ob)) continue;
        if(!(int)ob->query_property("e_clip")) continue;
        if(str != "all" && !(int)ob->id(str)) continue;
        max_ammo = (int)ob->query_property("ammo_max");
        cur_ammo = (int)ob->query_property("ammo_count");
        if(max_ammo <= 0) max_ammo = 30;
        if(cur_ammo >= max_ammo) continue;
        ob->set_property("ammo_count", max_ammo);
        count++;
    }

    if(!is_wiz) this_player()->add_money("credits", -total_cost);

    write("Sparks recharges " + count + " e-clip" +
        (count == 1 ? "" : "s") + " to full.\n");
    if(!is_wiz)
        write("Charged: " + total_cost + " credits.\n");
    else
        write("No charge for wizard staff.\n");
    return 1;
}
