// /d/Praxis/inn105.c
// Survivors' Hostel, Kitchen. Sells rations and water.

#include <std.h>
#include <rooms.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_property("rest_allowed", 1);
    set_short("Survivors' Hostel, Kitchen");
    set_long(
        "A working kitchen, which in the post-Rifts world means a gas burner\n"
        "that still lights, a counter made from two different tables, and a\n"
        "lockbox where Sal keeps the good ingredients. The smell of something\n"
        "warm and edible drifts from a pot on the burner.\n\n"
        "Basic rations and purified water are for sale. Type 'list' to see\n"
        "current stock. Type 'buy <item>' to purchase. Credits only.");
    set_exits( ([ "west" : "/domains/Praxis/inn102" ]) );
    set_listen("default",
        "The bubble of something cooking and the clank of cookware.");
    set_smell("default",
        "Something warm and starchy. Not fancy but real food.");
    set_items( ([
        "burner"   : "A single gas burner, still lit. The gas line is uncertain "
                     "but Sal has a backup tank.",
        "counter"  : "Two mismatched tables pushed together to make one long workspace.",
        "pot"      : "A battered pot with something bubbling in it. Smells edible.",
        "lockbox"  : "Where the better supplies are kept. Locked.",
        "shelves"  : "Metal shelving with sealed containers. Everything labeled."
    ]) );
}

void init() {
    ::init();
    add_action("cmd_buy",  "buy");
    add_action("cmd_list", "list");
}

int cmd_list(string str) {
    write("Kitchen stock:\n");
    write(" Item                             Price\n");
    write(" -------------------------------- ----------\n");
    write(sprintf(" %-32s %d credits\n", "field rations",  50));
    write(sprintf(" %-32s %d credits\n", "purified water", 20));
    return 1;
}

int cmd_buy(string str) {
    object ob;
    string s;
    int price;
    string item_name;
    string item_path;

    if(!str || !strlen(str)) {
        write("Buy what? Type 'list' to see kitchen stock.\n");
        return 1;
    }
    s = lower_case(str);
    ob = 0;
    price = 0;
    item_name = "";
    item_path = "";

    if(strsrch(s, "ration") != -1 || strsrch(s, "food") != -1) {
        item_path = EQ_PATH + "rations.c";
        item_name = "field rations";
        price = 50;
    } else if(strsrch(s, "water") != -1) {
        item_path = EQ_PATH + "purified_water.c";
        item_name = "purified water";
        price = 20;
    } else {
        write("The kitchen doesn't have that. Type 'list' to see stock.\n");
        return 1;
    }

    if((int)this_player()->query_money("credits") < price) {
        write("That costs " + price + " credits. You're a bit short.\n");
        return 1;
    }
    ob = clone_object(item_path);
    if(!ob) {
        write("Out of stock right now.\n");
        return 1;
    }
    this_player()->add_money("credits", -price);
    if(ob->move(this_player())) {
        write("You can't carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay " + price + " credits and receive " + item_name + ".\n");
    return 1;
}

void reset() { ::reset(); }
