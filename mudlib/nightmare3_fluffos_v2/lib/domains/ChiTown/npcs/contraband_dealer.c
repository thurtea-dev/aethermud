// /domains/ChiTown/npcs/contraband_dealer.c
// Black market dealer in the Burb alley.

#include <std.h>
#include <daemons.h>

#define ITEM_PATH "/domains/ChiTown/equipment/"
#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();

    set_name("contraband dealer");
    set_npc_name("Dealer");
    set_id( ({ "contraband dealer", "dealer", "figure", "man", "woman",
               "trader", "merchant" }) );
    set_short("a figure in a long coat");
    set_long(
        "A lean individual of indeterminate age wrapped in a long coat\n"
        "that conceals whatever is underneath. Eyes that have spent years\n"
        "watching for patrols. An easy, practiced calm that says this is\n"
        "not the first time they have stood in a dark alley.\n\n"
        "Say 'goods' to see what is available. Say 'buy <item>' to purchase.");
    set_level(4);
    set_max_hp(50);
    set_hp(50);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(-100);
    set_body_type("human");

    add_response("hello", "The dealer nods once. 'Looking for something?'");
    add_response("hi", "The dealer nods once. 'Looking for something?'");
    add_response("buy", "The dealer says, 'What do you want? Say goods to see the list.'");
    add_response("goods", "");
    add_response("contraband", "");
    add_response("credits", "The dealer says, 'Cash only. Coalition scrip welcome.'");
    add_response("bye", "The dealer says, 'Keep moving. Don't look back.'");
    set_default_response("The dealer watches you from the shadows without answering.");

    __items = ([
        "fake cs id" : ({ ITEM_PATH+"fake_cs_id.c", 2000,
                              "forged CS ID card" }),
        "fake id" : ({ ITEM_PATH+"fake_cs_id.c", 2000,
                              "forged CS ID card" }),
        "id" : ({ ITEM_PATH+"fake_cs_id.c", 2000,
                              "forged CS ID card" }),
        "e-clip" : ({ EQ_PATH+"e_clip.c", 500,
                              "black market e-clip" }),
        "eclip" : ({ EQ_PATH+"e_clip.c", 500,
                              "black market e-clip" }),
        "clip" : ({ EQ_PATH+"e_clip.c", 500,
                              "black market e-clip" })
    ]);
}

void show_goods() {
    write("The dealer opens the coat slightly:\n");
    write(" forged CS ID card 2000 credits\n");
    write(" black market e-clip 500 credits\n");
    write("Say 'buy <item>' to purchase.\n");
}

void catch_tell(string str) {
    string a, b;

    if(sscanf(str, "%s says: %s", a, b) != 2) {
        ::catch_tell(str);
        return;
    }
    b = lower_case(b);
    if(strsrch(b, "goods") != -1 || strsrch(b, "contraband") != -1 ||
       strsrch(b, "list") != -1 || strsrch(b, "sell") != -1) {
        call_out("show_goods", 1);
        return;
    }
    ::catch_tell(str);
}

void init() {
    ::init();
    add_action("cmd_buy", "buy");
}

int cmd_buy(string str) {
    string s;
    string *ks;
    mixed item;
    object ob;
    int i;
    int found;
    string key;

    if(!str || !strlen(str)) {
        write("Buy what? Say 'goods' to see available items.\n");
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
        say_here("Don't carry that. Say goods to see what I have.");
        return 1;
    }
    item = __items[key];
    if((int)this_player()->query_money("credits") < item[1]) {
        say_here("Not enough credits. That's " + item[1] + " credits.");
        return 1;
    }
    ob = clone_object(item[0]);
    if(!ob) {
        say_here("Out of stock on that right now.");
        return 1;
    }
    this_player()->add_money("credits", -item[1]);
    if(ob->move(this_player())) {
        write("You cannot carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay " + item[1] + " credits. The dealer hands you " +
          item[2] + ".\n");
    tell_room(environment(this_object()),
        "The dealer passes something to " +
        this_player()->query_cap_name() + " without a word.\n",
        ({ this_object(), this_player() }));
    return 1;
}
