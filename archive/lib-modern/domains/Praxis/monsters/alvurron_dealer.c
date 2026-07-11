/* /domains/Praxis/monsters/alvurron_dealer.c
   Undermarket dealer in Alvurron. Sells exotic Splynn goods for credits. */

#include <std.h>
#include <daemons.h>

#define EQ_SP "/domains/Praxis/equipment/splynn/"
#define EQ_PR "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();
    set_name("undermarket dealer");
    set_npc_name("Dealer");
    set_id( ({ "undermarket dealer", "dealer", "merchant" }) );
    set_short("a hooded undermarket dealer");
    set_long(
        "A hooded figure of indeterminate species works the alcove, hands\n"
        "gloved, voice a dry whisper. The dealer sells what the surface\n"
        "markets will not touch, and asks no questions in either\n"
        "direction.\n"
        "\nType 'list' to hear the whispered stock. 'buy <item>' to deal.");
    set_level(12);
    set_max_hp(200);
    set_hp(200);
    set_aggressive(0);
    set_moving(0);
    set_gender("neuter");
    set_race("unknown");
    set_alignment(-400);
    set_body_type("human");
    add_response("hello", "The dealer whispers: 'Buying, or browsing? Both cost time.'");
    add_response("bye", "The dealer whispers: 'You were never here. Neither was I.'");
    set_default_response("The dealer waits, gloved hands folded.");

    __items = ([
        "alien hide" : ({ EQ_SP+"alien_hide.c", 15000, "alien hide armor" }),
        "hide" : ({ EQ_SP+"alien_hide.c", 15000, "alien hide armor" }),
        "predator armor" : ({ EQ_SP+"predator_armor.c", 30000, "predator armor" }),
        "armor" : ({ EQ_SP+"predator_armor.c", 30000, "predator armor" }),
        "exotic energy clip" : ({ EQ_SP+"exotic_energy_clip.c", 4000, "exotic energy clip" }),
        "clip" : ({ EQ_SP+"exotic_energy_clip.c", 4000, "exotic energy clip" }),
        "net gun" : ({ EQ_SP+"net_gun.c", 8000, "net gun" }),
        "plasma pistol" : ({ EQ_SP+"plasma_pistol.c", 12000, "plasma pistol" }),
        "pistol" : ({ EQ_SP+"plasma_pistol.c", 12000, "plasma pistol" }),
        "ppe battery" : ({ EQ_PR+"ppe_battery.c", 6000, "PPE battery" }),
        "battery" : ({ EQ_PR+"ppe_battery.c", 6000, "PPE battery" })
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
    write("The dealer leans close and whispers the stock:\n");
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
        write("Buy what? Type 'list' for the stock.\n");
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
        write("The dealer whispers: 'Not in stock. Not for you, anyway.'\n");
        return 1;
    }
    item = __items[key];
    if((int)this_player()->query_money("credits") < item[1]) {
        write("The dealer whispers: '" + item[1] +
            " credits. Come back with them.'\n");
        return 1;
    }
    ob = clone_object(item[0]);
    if(!ob) {
        write("The dealer whispers: 'Gone. Someone got there first.'\n");
        return 1;
    }
    this_player()->add_money("credits", -item[1]);
    if(ob->move(this_player())) {
        write("You cannot carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay " + item[1] + " credits. A strongbox opens and the " +
        item[2] + " is yours.\n");
    tell_room(environment(this_object()),
        "The dealer passes " + this_player()->query_cap_name() +
        " something from a strongbox.\n", ({ this_object(), this_player() }));
    return 1;
}
