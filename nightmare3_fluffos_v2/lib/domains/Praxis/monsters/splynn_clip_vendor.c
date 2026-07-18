/* /domains/Praxis/monsters/splynn_clip_vendor.c
   Splynn Dimensional Market e-clip tech. Sells standard and long e-clips
   and recharges depleted ones on the spot.
   Commands: list, buy <item>, recharge <name|all> */

#include <std.h>
#include <daemons.h>

#define EQ_PATH  "/domains/Praxis/equipment/"
#define AMO_PATH "/domains/Praxis/equipment/ammo/"
#define RECHARGE_COST 50

inherit "/std/rifts_vendor";

void create() {
    ::create();
    set_name("clip tech");
    set_npc_name("the clip tech");
    set_id( ({ "clip tech", "tech", "clip vendor", "eclip vendor",
               "recharger", "vendor", "merchant" }) );
    set_short("a Splynn clip tech");
    set_long(
        "A wiry technician with grease-stained gloves and a portable charging\n"
        "rack bolted to a market stall. Indicator LEDs blink across a row of\n"
        "clip cradles. Standard and long e-clips sit in a locked cage under\n"
        "the counter.\n"
        "Type 'list' for stock. 'buy <item>' to purchase.\n"
        "'recharge all' to restore depleted e-clips you carry.\n"
    );
    set_level(5);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("neuter");
    set_race("unknown");
    set_alignment(-50);
    set_body_type("human");
    set_property("position_str", "tends a charging rack at a market stall.");

    add_response("hello", "The clip tech says: Clips or a recharge. Credits only.");
    add_response("hi", "The clip tech says: Clips or a recharge. Credits only.");
    add_response("list", "The clip tech says: Type 'list' to see stock.");
    add_response("buy", "The clip tech says: Type 'buy <item>' to purchase.");
    add_response("recharge", "The clip tech says: Type 'recharge all' or 'recharge <clip>'. Fifty credits each.");
    add_response("charge", "The clip tech says: Type 'recharge all' or 'recharge <clip>'.");
    add_response("sell", "The clip tech says: I sell. I recharge. I do not buy.");
    add_response("bye", "The clip tech says: Keep your magazines topped.");
    add_response("help", "The clip tech says: 'list', 'buy <name>', 'recharge all'.");
    set_default_response("The clip tech checks a clip indicator and waits.");
    set_shop_sells(0);

    set_shop_items( ([
        "e-clip"         : ({ EQ_PATH+"e_clip.c",           350, "standard e-clip (30 shots)" }),
        "eclip"          : ({ EQ_PATH+"e_clip.c",           350, "standard e-clip (30 shots)" }),
        "clip"           : ({ EQ_PATH+"e_clip.c",           350, "standard e-clip (30 shots)" }),
        "long e-clip"    : ({ AMO_PATH+"e_clip_long.c",     550, "long e-clip (60 shots)" }),
        "long clip"      : ({ AMO_PATH+"e_clip_long.c",     550, "long e-clip (60 shots)" }),
        "standard e-clip": ({ AMO_PATH+"e_clip_standard.c", 350, "standard e-clip" }),
        "standard clip"  : ({ AMO_PATH+"e_clip_standard.c", 350, "standard e-clip" })
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

    if(!str || !sizeof(str)) {
        write("Recharge what? Try 'recharge all' or 'recharge <clip name>'.\n");
        return 1;
    }
    str = lower_case(str);
    inv = all_inventory(this_player());

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

    total_cost = count * RECHARGE_COST;
    if((int)this_player()->query_money("credits") < total_cost) {
        say_here("Recharging " + count + " clip" +
            (count == 1 ? "" : "s") + " costs " + total_cost +
            " credits. You are short.");
        return 1;
    }

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

    this_player()->add_money("credits", -total_cost);
    write("The clip tech recharges " + count + " e-clip" +
        (count == 1 ? "" : "s") + " to full.\n");
    write("Charged: " + total_cost + " credits.\n");
    return 1;
}
