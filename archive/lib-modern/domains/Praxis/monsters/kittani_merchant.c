// /d/Praxis/monsters/kittani_merchant.c
// Kittani merchant -- non-aggressive exotic goods vendor in Splynn Market.

#include <std.h>
#include <daemons.h>

#define SPLYNN_EQ "/domains/Praxis/equipment/splynn/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();

    set_name("Kittani merchant");
    set_npc_name("the Kittani merchant");
    set_id( ({ "kittani merchant", "kittani", "merchant", "vendor", "trader", "alien merchant" }) );
    set_short("a Kittani merchant");
    set_long(
        "A Kittani in civilian attire, still reptilian and unsettling,\n"
        "but without power armor. Layered robes of dimensional silk cover\n"
        "a small frame. Golden eyes track your movements with mercantile\n"
        "precision. Behind the stall counter, shelves hold items that\n"
        "should not exist in this dimension.\n"
        "\nTry 'list' or 'buy <item>'.");
    set_level(4);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("kittani");
    set_alignment(-100);
    set_body_type("humanoid");

    add_response("hello", "The Kittani merchant inclines its head. 'Welcome, soft-skin. Credits acceptable.'");
    add_response("hi", "The Kittani merchant inclines its head. 'Welcome, soft-skin. Credits acceptable.'");
    add_response("list", "The Kittani merchant says: 'Examine my wares. Type list.'");
    add_response("buy", "The Kittani merchant says: 'Specify item. Type buy <item>.'");
    add_response("sell", "The Kittani merchant says: 'I do not purchase from dimension-walkers. Only sell.'");
    add_response("help", "The Kittani merchant says: 'List. Buy. Credits. Simple.'");
    add_response("bye", "The Kittani merchant says: 'Return when credits are plentiful.'");
    set_default_response("The Kittani merchant regards you with golden eyes and says nothing useful.");

    __items = ([
        "exotic energy clip" : ({ SPLYNN_EQ+"exotic_energy_clip.c", 800, "exotic double-capacity e-clip" }),
        "exotic clip" : ({ SPLYNN_EQ+"exotic_energy_clip.c", 800, "exotic double-capacity e-clip" }),
        "double clip" : ({ SPLYNN_EQ+"exotic_energy_clip.c", 800, "exotic double-capacity e-clip" }),
        "psi sword hilt" : ({ SPLYNN_EQ+"psi_sword_hilt.c", 5000, "psi-sword hilt" }),
        "psi hilt" : ({ SPLYNN_EQ+"psi_sword_hilt.c", 5000, "psi-sword hilt" }),
        "hilt" : ({ SPLYNN_EQ+"psi_sword_hilt.c", 5000, "psi-sword hilt" }),
        "splynn map" : ({ SPLYNN_EQ+"splynn_map.c", 200, "map of Splynn district" }),
        "map" : ({ SPLYNN_EQ+"splynn_map.c", 200, "map of Splynn district" })
    ]);
}

void init() {
    ::init();
    add_action("cmd_buy", "buy");
    add_action("cmd_list", "list");
}

int cmd_list(string str) {
    string *ks, seen;
    mixed item;
    int i;

    seen = "";
    write("The Kittani merchant displays wares --\n");
    write(" Item Price\n");
    write(" -------------------------------- ----------\n");
    ks = keys(__items);
    i = sizeof(ks);
    while(i--) {
        item = __items[ks[i]];
        if(strsrch(seen, item[2]) != -1) continue;
        seen += item[2];
        write(sprintf(" %-32s %d credits\n", item[2], item[1]));
    }
    return 1;
}

int cmd_buy(string str) {
    string *ks, key, s;
    mixed item;
    object ob;
    int i, found;

    if(!str || !strlen(str)) {
        write("Buy what? Type 'list' to see available goods.\n");
        return 1;
    }
    s = lower_case(str);
    ks = keys(__items);
    i = sizeof(ks);
    found = 0;
    key = "";
    while(i--) {
        if(strsrch(s, ks[i]) != -1 || strsrch(ks[i], s) != -1) {
            key = ks[i];
            found = 1;
            break;
        }
    }
    if(!found) {
        say_here("I do not carry that. Type list.");
        return 1;
    }
    item = __items[key];
    if((int)this_player()->query_money("credits") < item[1]) {
        say_here("Insufficient credits. That is "+item[1]+" credits.");
        return 1;
    }
    ob = clone_object(item[0]);
    if(!ob) {
        say_here("That item is not available.");
        return 1;
    }
    this_player()->add_money("credits", -item[1]);
    if(ob->move(this_player())) {
        write("You cannot carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay "+item[1]+" credits. The merchant hands you "+item[2]+".\n");
    tell_room(environment(this_object()),
        "The Kittani merchant hands "+this_player()->query_cap_name()+" "+item[2]+".\n",
        ({ this_object(), this_player() }));
    return 1;
}
