// /domains/chitown/npcs/cs_quartermaster.c
// CS Quartermaster vendor NPC.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();

    set_name("CS quartermaster");
    set_npc_name("Quartermaster");
    set_id( ({ "cs quartermaster", "quartermaster", "vendor", "soldier" }) );
    set_short("the CS Quartermaster");
    set_long(
        "A broad-shouldered Coalition logistics officer in pressed gray\n"
        "dress uniform. Rank insignia on the shoulders. A datapad in hand\n"
        "and the air of someone who has processed a thousand requisitions.\n"
        "\nType 'list' to see available supplies. 'buy <item>' to purchase.");
    set_level(5);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(-100);
    set_body_type("human");

    add_response("hello", "Quartermaster says: 'State your requisition, citizen.'");
    add_response("hi", "Quartermaster says: 'State your requisition, citizen.'");
    add_response("list", "Quartermaster says: 'Type list to see available stock.'");
    add_response("buy", "Quartermaster says: 'Type buy <item> to requisition.'");
    add_response("sell", "Quartermaster says: 'We do not buy from civilians.'");
    add_response("bye", "Quartermaster says: 'Dismissed.'");
    add_response("work", "");
    add_response("job", "");
    add_response("mission", "");
    add_response("done", "");
    set_default_response("The quartermaster eyes you with practiced suspicion.");

    __items = ([
        "dead boy armor" : ({ EQ_PATH+"dead_boy_armor.c", 15000, "Dead Boy armor (CA-4)" }),
        "dead boy" : ({ EQ_PATH+"dead_boy_armor.c", 15000, "Dead Boy armor (CA-4)" }),
        "armor" : ({ EQ_PATH+"dead_boy_armor.c", 15000, "Dead Boy armor (CA-4)" }),
        "cp-40" : ({ EQ_PATH+"cp40_laser.c", 12000, "CP-40 laser rifle" }),
        "cp40" : ({ EQ_PATH+"cp40_laser.c", 12000, "CP-40 laser rifle" }),
        "rifle" : ({ EQ_PATH+"cp40_laser.c", 12000, "CP-40 laser rifle" }),
        "cp-30" : ({ EQ_PATH+"cp30_laser.c", 6000, "CP-30 laser pistol" }),
        "cp30" : ({ EQ_PATH+"cp30_laser.c", 6000, "CP-30 laser pistol" }),
        "pistol" : ({ EQ_PATH+"cp30_laser.c", 6000, "CP-30 laser pistol" }),
        "rations" : ({ EQ_PATH+"rations.c", 100, "military rations" }),
        "ration" : ({ EQ_PATH+"rations.c", 100, "military rations" }),
        "field kit" : ({ EQ_PATH+"first_aid_kit.c", 300, "CS field kit" }),
        "kit" : ({ EQ_PATH+"first_aid_kit.c", 300, "CS field kit" }),
        "c-12" : ({ EQ_PATH+"c12_laser_rifle.c", 22000, "C-12 heavy laser rifle" }),
        "c12" : ({ EQ_PATH+"c12_laser_rifle.c", 22000, "C-12 heavy laser rifle" }),
        "boom gun" : ({ EQ_PATH+"boom_gun.c", 45000, "Boom Gun rail weapon" }),
        "terror trooper" : ({ EQ_PATH+"power_armor/CS_Terror_Trooper.c", 85000,
            "CS Terror Trooper power armor (officer requisition)" }),
        "terror trooper armor" : ({ EQ_PATH+"power_armor/CS_Terror_Trooper.c", 85000,
            "CS Terror Trooper power armor" })
    ]);
}

void init() {
    ::init();
    add_action("cmd_buy", "buy");
    add_action("cmd_list", "list");
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says: %s", a, b) != 2) return;
    b = lower_case(b);
    tp = this_player();

    if(strsrch(b, "work") != -1 || strsrch(b, "job") != -1 ||
       strsrch(b, "mission") != -1) {
        if(!tp || !objectp(tp)) return;
        call_out("offer_stolen_goods", 1, tp);
        return;
    }
    if(strsrch(b, "done") != -1) {
        if(!tp || !objectp(tp)) return;
        call_out("check_stolen_goods_done", 1, tp);
        return;
    }
    ::catch_tell(str);
}

void offer_stolen_goods(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "stolen_goods") ||
       (int)QUEST_D->is_quest_active(tp, "stolen_goods")) {
        say_here("Quartermaster says: 'Your orders stand. Bandits east of Illinois Road.'");
        return;
    }
    say_here("Quartermaster says: 'Soldier. We had a weapons shipment hit on the highway. "
             "Bandits. Find them and deal with it. East of Illinois Road.'");
    catch(QUEST_D->start_quest(tp, "stolen_goods"));
}

void check_stolen_goods_done(object tp) {
    mapping aq;
    mapping prog;

    if(!tp || !objectp(tp)) return;
    if(!(int)QUEST_D->is_quest_active(tp, "stolen_goods")) {
        say_here("Quartermaster says: 'I have no outstanding orders for you.'");
        return;
    }
    aq = (mapping)QUEST_D->query_active_quests(tp);
    prog = (mapping)aq["stolen_goods"];
    if(!prog || (int)prog["bandits_killed"] < 3) {
        say_here("Quartermaster says: 'You are not finished. Three bandits. East of Illinois Road.'");
        return;
    }
    if(!(int)QUEST_D->complete_quest(tp, "stolen_goods")) {
        say_here("Quartermaster says: 'Report logged. Stand by.'");
        return;
    }
    catch(FACTION_D->modify_standing(
        (string)tp->query_name(), "coalition", 50, "Quest: stolen_goods"));
    say_here("Quartermaster says: 'Good work. The Coalition thanks you. Dismissed.'");
}

int cmd_list(string str) {
    string *ks;
    mixed item;
    string seen;
    int i;

    if((int)WANTED_D->is_cs_enemy(this_player())) {
        say_here("Quartermaster says: 'You are not authorized to requisition CS equipment.'");
        return 1;
    }
    seen = "";
    write("Quartermaster says: 'Current stock follows.'\n");
    write(" Item Price\n");
    write(" ----------------------------------- ----------\n");
    ks = keys(__items);
    i = sizeof(ks);
    while(i--) {
        item = __items[ks[i]];
        if(strsrch(seen, item[2]) != -1) continue;
        seen += item[2];
        write(sprintf(" %-35s %d credits\n", item[2], item[1]));
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

    if((int)WANTED_D->is_cs_enemy(this_player())) {
        say_here("Quartermaster says: 'You are not authorized to requisition CS equipment.'");
        return 1;
    }
    if(!str || !strlen(str)) {
        write("Buy what? Type 'list' to see available stock.\n");
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
        say_here("Insufficient credits. That item costs " + item[1] + " credits.");
        return 1;
    }
    ob = clone_object(item[0]);
    if(!ob) {
        say_here("Out of stock on that item right now.");
        return 1;
    }
    this_player()->add_money("credits", -item[1]);
    if(ob->move(this_player())) {
        write("You cannot carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay " + item[1] + " credits. The quartermaster hands you " + item[2] + ".\n");
    tell_room(environment(this_object()),
        "The quartermaster hands " + this_player()->query_cap_name() + " " + item[2] + ".\n",
        ({ this_object(), this_player() }));
    return 1;
}
