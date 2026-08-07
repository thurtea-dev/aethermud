/* /domains/Horton/npcs/horton_shopkeeper.c
   Horton general store proprietor -- sells survival basics, no questions asked. */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"
#define HORTON_PATH "/domains/Horton/equipment/"

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("shopkeeper");
    set_npc_name("shopkeeper");
    set_id( ({ "shopkeeper", "proprietor", "merchant", "seller", "man",
               "horton shopkeeper", "shop keeper" }) );
    set_short("the store proprietor");
    set_long(
        "Compact and quiet. Somewhere between forty and sixty years old, which\n"
        "in Rifts Earth means they have survived a great deal. They know every\n"
        "item on the shelves and its price to the credit. They will not ask where\n"
        "you are going or what you need it for. Type 'list' to see stock.\n"
        "Type 'buy <item>' to purchase.");
    set_level(3);
    set_max_hp(50);
    set_hp(50);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(50);
    set_body_type("human");

    add_response("hello",
        "shopkeeper says: 'Got most things you need to stay alive. Type list.'");
    add_response("hi",
        "The proprietor nods. 'Type list to see what we have.'");
    add_response("stock",   "");
    add_response("inventory","");
    add_response("price",   "");
    add_response("cost",    "");
    add_response("help",
        "shopkeeper says: 'Type list to see available goods. Buy item name to purchase.'");
    add_response("buy",     "");
    add_response("bye",     "shopkeeper says: 'Watch yourself out there.'");
    set_default_response("The proprietor listens and waits.");
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
        "The proprietor slides a handwritten price list across the counter:\n"
        "  rations         emergency field rations         50 credits\n"
        "  rope            30ft climbing rope              40 credits\n"
        "  knife           all-purpose blade              120 credits\n"
        "  radio           personal field radio           300 credits\n"
        "  medkit          basic medical kit              100 credits\n"
        "\nType: buy rations / buy rope / buy knife / buy radio / buy medkit\n",
        ({ this_object() }));
}

void init() {
    ::init();
    add_action("cmd_buy",  "buy");
    add_action("cmd_list", "list");
}

int cmd_list(string str) {
    call_out("show_stock", 0);
    return 1;
}

int cmd_buy(string str) {
    object item;
    int cost;
    string item_file;
    string item_name;
    int use_horton;

    if(!str) {
        write("Buy what? Type 'list' to see available items.\n");
        return 1;
    }
    str = lower_case(str);
    use_horton = 0;

    if(strsrch(str, "ration") != -1) {
        item_file = "rations.c";
        item_name = "emergency rations";
        cost = 50;
    } else if(strsrch(str, "rope") != -1) {
        item_file = "rope.c";
        item_name = "rope";
        cost = 40;
    } else if(strsrch(str, "knife") != -1) {
        item_file = "knife.c";
        item_name = "knife";
        cost = 120;
    } else if(strsrch(str, "radio") != -1) {
        item_file = "personal_radio.c";
        item_name = "personal radio";
        cost = 300;
    } else if(strsrch(str, "medkit") != -1 || strsrch(str, "med kit") != -1 ||
              strsrch(str, "medical") != -1) {
        item_file = "basic_medkit.c";
        item_name = "basic medkit";
        cost = 100;
        use_horton = 1;
    } else {
        write("The store does not carry that. Type 'list' to see available items.\n");
        return 1;
    }

    if((int)this_player()->query_money("credits") < cost) {
        say_here("You need " + cost + " credits for that.");
        return 1;
    }
    this_player()->add_money("credits", -cost);
    if(use_horton)
        item = clone_object(HORTON_PATH + item_file);
    else
        item = clone_object(EQ_PATH + item_file);
    if(item) {
        item->move(this_player());
        tell_object(this_player(),
            "The proprietor takes " + cost + " credits and hands you the " +
            item_name + ".\n");
        tell_room(environment(this_object()),
            "The proprietor completes a quick transaction with " +
            (string)this_player()->query_cap_name() + ".\n",
            ({ this_object(), this_player() }));
    }
    return 1;
}
