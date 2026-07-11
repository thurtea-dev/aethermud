/* /std/rifts_vendor.c
   Credit-based shop mixin for Rifts NPC merchants.
   Item mapping: keyword -> ({ file_path, price, display_name })
   Currency defaults to credits. Stock is cloned on purchase (infinite supply). */

#include <std.h>

inherit "/std/rifts_npc";

private mapping __shop_items;
private int __shop_sells;
private string __shop_currency;

void create() {
    ::create();
    __shop_items = ([]);
    __shop_sells = 1;
    __shop_currency = "credits";
    set_property("rifts_no_flee", 1);
}

void set_shop_items(mapping items) {
    if(!items) items = ([]);
    __shop_items = items;
}

void set_shop_sells(int flag) {
    __shop_sells = flag ? 1 : 0;
}

void set_shop_currency(string curr) {
    if(!curr || !sizeof(curr)) curr = "credits";
    __shop_currency = curr;
}

mapping query_shop_items() {
    return __shop_items;
}

int query_shop_sells() {
    return __shop_sells;
}

string query_shop_currency() {
    return __shop_currency;
}

void init() {
    ::init();
    add_action("shop_list", "list");
    add_action("shop_buy", "buy");
    if(__shop_sells) {
        add_action("shop_sell", "sell");
        add_action("shop_value", "value");
    }
}

private string find_shop_key(string str) {
    string *ks;
    string s;
    int i;

    if(!str || !sizeof(str)) return "";
    if(!__shop_items || !sizeof(__shop_items)) return "";
    s = lower_case(str);
    ks = keys(__shop_items);
    i = sizeof(ks);
    while(i--) {
        if(strsrch(s, ks[i]) != -1 || strsrch(ks[i], s) != -1)
            return ks[i];
    }
    return "";
}

private mixed query_shop_entry(string key) {
    if(!key || !sizeof(key)) return 0;
    return __shop_items[key];
}

private int sell_offer(object ob) {
    int val;
    int offer;

    val = (int)ob->query_value();
    if(val <= 0) return 0;
    offer = val / 2;
    if(offer < 1) offer = 1;
    return offer;
}

int shop_list(string str) {
    string *ks;
    string seen;
    mixed item;
    int i;

    if(!__shop_items || !sizeof(__shop_items)) {
        say_here("I have nothing for sale right now.");
        return 1;
    }
    seen = "";
    write(query_npc_name() + " says: Here's what I carry:\n");
    write(" Item                             Price\n");
    write(" -------------------------------- ----------\n");
    ks = keys(__shop_items);
    i = sizeof(ks);
    while(i--) {
        item = __shop_items[ks[i]];
        if(!item || !sizeof(item) || sizeof(item) < 3) continue;
        if(strsrch(seen, item[2]) != -1) continue;
        seen += item[2];
        write(sprintf(" %-32s %d %s\n", item[2], item[1], __shop_currency));
    }
    return 1;
}

int shop_buy(string str) {
    string key;
    mixed item;
    object ob;
    object buyer;
    object env;
    int price;

    if(!str || !sizeof(str)) {
        write("Buy what? Type 'list' to see available goods.\n");
        return 1;
    }
    key = find_shop_key(str);
    if(!key || !sizeof(key)) {
        say_here("I don't carry that. Type 'list' to see what I've got.");
        return 1;
    }
    item = query_shop_entry(key);
    if(!item || sizeof(item) < 3) {
        say_here("That item is not available right now.");
        return 1;
    }
    buyer = this_player();
    price = item[1];
    if((int)buyer->query_money(__shop_currency) < price) {
        say_here("You're a bit short. That costs " + price + " " +
            __shop_currency + ".");
        return 1;
    }
    ob = clone_object(item[0]);
    if(!ob) {
        say_here("I'm out of that right now.");
        return 1;
    }
    buyer->add_money(__shop_currency, -price);
    if(ob->move(buyer)) {
        buyer->add_money(__shop_currency, price);
        destruct(ob);
        write("You can't carry that.\n");
        return 1;
    }
    write("You pay " + price + " " + __shop_currency + ". " +
        query_npc_name() + " hands you " + item[2] + ".\n");
    env = environment(this_object());
    if(env) {
        tell_room(env,
            query_npc_name() + " hands " + (string)buyer->query_cap_name() +
            " " + item[2] + ".\n",
            ({ this_object(), buyer }));
    }
    return 1;
}

int shop_value(string str) {
    object ob;
    int offer;

    if(!__shop_sells) {
        say_here("I don't buy goods here.");
        return 1;
    }
    if(!str || !sizeof(str)) {
        write("Value what? Name an item you're carrying.\n");
        return 1;
    }
    ob = present(str, this_player());
    if(!ob) {
        say_here("I don't see that on you.");
        return 1;
    }
    offer = sell_offer(ob);
    if(offer <= 0) {
        say_here("I wouldn't give you a credit for that.");
        return 1;
    }
    say_here("I'd give you " + offer + " " + __shop_currency + " for " +
        (string)ob->query_short() + ".");
    return 1;
}

int shop_sell(string str) {
    object ob;
    int offer;

    if(!__shop_sells) {
        say_here("I don't buy goods here.");
        return 1;
    }
    if(!str || !sizeof(str)) {
        write("Sell what? Name an item you're carrying.\n");
        return 1;
    }
    ob = present(str, this_player());
    if(!ob) {
        say_here("I don't see that on you.");
        return 1;
    }
    offer = sell_offer(ob);
    if(offer <= 0) {
        say_here("I wouldn't give you a credit for that.");
        return 1;
    }
    ob->move(this_object());
    this_player()->add_money(__shop_currency, offer);
    write(query_npc_name() + " pays you " + offer + " " + __shop_currency +
        " for " + (string)ob->query_short() + ".\n");
    tell_room(environment(this_object()),
        (string)this_player()->query_cap_name() + " sells " +
        (string)ob->query_short() + " to " + query_npc_name() + ".\n",
        ({ this_object(), this_player() }));
    return 1;
}
