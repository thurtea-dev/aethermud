// /domains/NewCamelot/npcs/camelot_armourer.c
// New Camelot armourer vendor NPC.

#include <std.h>
#include <daemons.h>

#define EQ_PATH_NC "/domains/NewCamelot/equipment/"
#define EQ_PATH_PR "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();

    set_name("camelot armorer");
    set_npc_name("Armorer");
    set_id( ({ "camelot armorer", "camelot armourer", "armorer", "armourer", "smith", "blacksmith" }) );
    set_short("the Camelot Armorer");
    set_long(
        "A broad woman with scarred forearms and the steady gaze of someone\n"
        "who has been working metal for decades. Her apron is blackened with\n"
        "forge smoke. She examines your gear with a professional eye.\n"
        "\nType 'list' to see available goods. 'buy <item>' to purchase.");
    set_level(5);
    set_max_hp(100);
    set_hp(100);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(200);
    set_body_type("human");

    add_response("hello", "Armorer says: 'Good gear keeps you alive. Take a look at my stock.'");
    add_response("hi", "Armorer says: 'Good gear keeps you alive. Take a look at my stock.'");
    add_response("list", "Armorer says: 'Type list to see what I carry.'");
    add_response("buy", "Armorer says: 'Type buy <item> to purchase.'");
    add_response("sell", "Armorer says: 'I only buy good quality work. Not interested in scrap.'");
    add_response("bye", "Armorer says: 'Stay sharp. Both figuratively and literally.'");
    set_default_response("The armorer eyes your equipment appraisingly.");

    __items = ([
        "vibro-sword" : ({ EQ_PATH_PR+"vibro_sword.c", 5000, "vibro-sword" }),
        "vibro sword" : ({ EQ_PATH_PR+"vibro_sword.c", 5000, "vibro-sword" }),
        "vibro-knife" : ({ EQ_PATH_PR+"vibro_knife.c", 2000, "vibro-knife" }),
        "vibro knife" : ({ EQ_PATH_PR+"vibro_knife.c", 2000, "vibro-knife" }),
        "chain mail" : ({ EQ_PATH_NC+"chain_mail.c", 1500, "chain mail" }),
        "chainmail" : ({ EQ_PATH_NC+"chain_mail.c", 1500, "chain mail" }),
        "short sword" : ({ EQ_PATH_NC+"short_sword.c", 800, "short sword" }),
        "shortsword" : ({ EQ_PATH_NC+"short_sword.c", 800, "short sword" }),
        "sword" : ({ EQ_PATH_NC+"short_sword.c", 800, "short sword" }),
        "knight shield": ({ EQ_PATH_NC+"knight_shield.c", 1200, "knight shield" }),
        "shield" : ({ EQ_PATH_NC+"knight_shield.c", 1200, "knight shield" })
    ]);
}

void init() {
    ::init();
    add_action("cmd_buy",   "buy");
    add_action("cmd_list",  "list");
    add_action("cmd_sell",  "sell");
    add_action("cmd_value", "value");
}

int cmd_list(string str) {
    string *ks;
    mixed item;
    string seen;
    int i;

    seen = "";
    write("Armorer says: 'Here is what I carry.'\n");
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
    string *ks;
    string s;
    mixed item;
    object ob;
    int i;
    int found;
    string key;

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
        say_here("I don't carry that. Type 'list' for current stock.");
        return 1;
    }
    item = __items[key];
    if((int)this_player()->query_money("credits") < item[1]) {
        say_here("That costs " + item[1] + " credits. You're a bit short.");
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
    write("You pay " + item[1] + " credits. The armorer hands you " + item[2] + ".\n");
    tell_room(environment(this_object()),
        "The armorer hands " + this_player()->query_cap_name() + " " + item[2] + ".\n",
        ({ this_object(), this_player() }));
    return 1;
}

int cmd_value(string str) {
    object ob;
    int val;
    int offer;

    if(!str || !strlen(str)) {
        write("Value what? Name an item you're carrying.\n");
        return 1;
    }
    ob = present(str, this_player());
    if(!ob) {
        say_here("I don't see that on you.");
        return 1;
    }
    val = (int)ob->query_value();
    if(val <= 0) {
        say_here("That's not worth anything to me.");
        return 1;
    }
    offer = val / 2;
    if(offer < 1) offer = 1;
    say_here("I'd give you " + offer + " credits for " +
        (string)ob->query_short() + ".");
    return 1;
}

int cmd_sell(string str) {
    object ob;
    int val;
    int offer;

    if(!str || !strlen(str)) {
        write("Sell what? Name an item you're carrying.\n");
        return 1;
    }
    ob = present(str, this_player());
    if(!ob) {
        say_here("I don't see that on you.");
        return 1;
    }
    val = (int)ob->query_value();
    if(val <= 0) {
        say_here("I wouldn't give you a credit for that.");
        return 1;
    }
    offer = val / 2;
    if(offer < 1) offer = 1;
    ob->move(this_object());
    this_player()->add_money("credits", offer);
    write("The armorer pays you " + offer + " credits for " +
        (string)ob->query_short() + ".\n");
    tell_room(environment(this_object()),
        this_player()->query_cap_name() + " sells " +
        (string)ob->query_short() + " to the armorer.\n",
        ({ this_object(), this_player() }));
    return 1;
}
