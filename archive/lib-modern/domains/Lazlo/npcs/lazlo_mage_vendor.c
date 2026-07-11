// /domains/Lazlo/npcs/lazlo_mage_vendor.c
// Lazlo magic quarter vendor -- sells PPE items and spell components.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();

    set_name("Lazlo mage-vendor");
    set_npc_name("Mage-Vendor");
    set_id( ({ "lazlo mage-vendor", "lazlo mage vendor", "mage-vendor",
               "mage vendor", "vendor", "merchant" }) );
    set_short("a Lazlo mage-vendor");
    set_long(
        "A slight human in deep violet robes with the faint smell of burnt PPE\n"
        "about them. Shelves behind the counter are packed with labeled jars,\n"
        "sealed cases, and things floating in amber liquid. The vendor's eyes\n"
        "are a little too focused, the way people get who work around active\n"
        "ley lines for too many years.\n"
        "\nType 'list' to see available goods. 'buy <item>' to purchase.");

    set_level(6);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_race("human");
    set_alignment(150);
    set_body_type("human");

    add_response("hello",
        "Mage-Vendor says: 'Welcome. The components are fresh. "
        "The PPE batteries were charged at the nexus this morning.'");
    add_response("hi",
        "Mage-Vendor says: 'Welcome. The components are fresh. "
        "The PPE batteries were charged at the nexus this morning.'");
    add_response("list", "Mage-Vendor says: 'Type list to see current stock.'");
    add_response("buy",  "Mage-Vendor says: 'Type buy <item> to purchase.'");
    add_response("sell", "Mage-Vendor says: 'I do not buy. Components must meet standards.'");
    add_response("ppe",
        "Mage-Vendor says: 'PPE batteries store 30 PPE, charged at the nexus weekly. "
        "Used by techno-wizards and enchanted devices.'");
    add_response("spell",
        "Mage-Vendor says: 'Components are ground and prepared in-house. "
        "Purity is the difference between a working ritual and an incident.'");
    add_response("ritual",
        "Mage-Vendor says: 'Ritual components for advanced ceremonies. "
        "I source them carefully.'");
    add_response("bye", "Mage-Vendor says: 'May your PPE recover quickly.'");
    set_default_response("The mage-vendor waits with patient attention.");

    __items = ([
        "ppe battery"      : ({ EQ_PATH+"ppe_battery.c",        500, "PPE battery (30 PPE)" }),
        "battery"          : ({ EQ_PATH+"ppe_battery.c",        500, "PPE battery (30 PPE)" }),
        "spell components" : ({ EQ_PATH+"spell_components.c",   200, "spell components" }),
        "spell"            : ({ EQ_PATH+"spell_components.c",   200, "spell components" }),
        "ritual components": ({ EQ_PATH+"ritual_components.c",  300, "ritual components" }),
        "ritual"           : ({ EQ_PATH+"ritual_components.c",  300, "ritual components" })
    ]);
}

void init() {
    ::init();
    add_action("cmd_buy",  "buy");
    add_action("cmd_list", "list");
}

int cmd_list(string str) {
    string *ks;
    mixed item;
    string seen;
    int i;

    seen = "";
    write("Mage-Vendor says: 'Current stock:'\n");
    write(" Item                             Price\n");
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
        say_here("That costs " + item[1] + " credits. You're short.");
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
    write("You pay " + item[1] + " credits. The mage-vendor hands you " + item[2] + ".\n");
    tell_room(environment(this_object()),
        "The mage-vendor hands " + this_player()->query_cap_name() +
        " " + item[2] + ".\n",
        ({ this_object(), this_player() }));
    return 1;
}
