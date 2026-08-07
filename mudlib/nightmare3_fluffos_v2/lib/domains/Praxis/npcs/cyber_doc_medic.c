// /domains/Praxis/npcs/cyber_doc_medic.c
// Cyber-doc surgeon for bionic installation.

#include <std.h>
#include <daemons.h>

#define CYBER_EQ "/domains/Praxis/equipment/cybernetics/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();
    set_name("cyber-doc");
    set_npc_name("Doc");
    set_id(({ "cyber-doc", "cyber doc", "doc", "surgeon", "doctor" }));
    set_short("a cyber-doc surgeon");
    set_long(
        "A gaunt surgeon in blood-spotted scrubs with chrome-plated fingers.\n"
        "Implant diagrams cover the walls. Type 'list' to see implants for sale,\n"
        "'buy <implant>' to purchase one, then 'install <cybernetic>' while\n"
        "carrying a cybernetic item. Installation costs half the item value in credits.");
    set_level(8);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(0);
    set_body_type("human");
    add_response("hello", "Doc says: 'Drop the chrome on the table and hold still.'");
    add_response("help", "Doc says: 'List. Buy. Install. In that order, usually.'");
    set_default_response("The cyber-doc adjusts a laser scalpel and waits.");

    __items = ([
        "gyro compass implant" : ({ CYBER_EQ+"gyro_compass_implant.c", 1500, "internal gyro-compass implant" }),
        "gyro compass" : ({ CYBER_EQ+"gyro_compass_implant.c", 1500, "internal gyro-compass implant" }),
        "compass" : ({ CYBER_EQ+"gyro_compass_implant.c", 1500, "internal gyro-compass implant" }),
        "oxygen storage cell" : ({ CYBER_EQ+"oxygen_cell.c", 2500, "oxygen storage cell" }),
        "oxygen cell" : ({ CYBER_EQ+"oxygen_cell.c", 2500, "oxygen storage cell" }),
        "o2 cell" : ({ CYBER_EQ+"oxygen_cell.c", 2500, "oxygen storage cell" }),
        "neural net implant" : ({ CYBER_EQ+"neural_net_implant.c", 15000, "neural net implant" }),
        "neural net" : ({ CYBER_EQ+"neural_net_implant.c", 15000, "neural net implant" }),
        "neural implant" : ({ CYBER_EQ+"neural_net_implant.c", 15000, "neural net implant" }),
        "irmss medical system" : ({ CYBER_EQ+"irmss_kit.c", 25000, "IRMSS medical system" }),
        "irmss kit" : ({ CYBER_EQ+"irmss_kit.c", 25000, "IRMSS medical system" }),
        "irmss" : ({ CYBER_EQ+"irmss_kit.c", 25000, "IRMSS medical system" })
    ]);
}

void init() {
    ::init();
    add_action("cmd_install", "install");
    add_action("cmd_buy", "buy");
    add_action("cmd_list", "list");
}

int cmd_list(string str) {
    string *ks, seen;
    mixed item;
    int i;

    seen = "";
    write("Doc gestures at a case of implants --\n");
    write(" Implant Price\n");
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
        write("Buy what? Type 'list' to see available implants.\n");
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
        write("Doc says: 'Not something I carry. Type list.'\n");
        return 1;
    }
    item = __items[key];
    if((int)this_player()->query_money("credits") < item[1]) {
        write("Doc says: 'That runs " + item[1] + " credits.'\n");
        return 1;
    }
    ob = clone_object(item[0]);
    if(!ob) {
        write("Doc says: 'Out of stock on that, actually.'\n");
        return 1;
    }
    this_player()->add_money("credits", -item[1]);
    if(ob->move(this_player())) {
        write("You cannot carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay " + item[1] + " credits. Doc hands you " + item[2] + ".\n");
    tell_room(environment(this_object()),
        "Doc hands " + this_player()->query_cap_name() + " " + item[2] + ".\n",
        ({ this_object(), this_player() }));
    return 1;
}

int cmd_install(string str) {
    object ob;
    object doc;
    string bionic_name;
    string slot;
    mapping bdef;
    mapping installed;
    int cost;

    doc = this_object();
    if(environment(this_player()) != environment(doc)) return 0;
    if(!present(doc, environment(this_player()))) return 0;
    if(!str || !sizeof(str))
        return notify_fail("Syntax: install <cybernetic item>\n");
    ob = present(lower_case(str), this_player());
    if(!ob)
        return notify_fail("You are not carrying that.\n");
    if(!(int)ob->query_property("cybernetic"))
        return notify_fail("Doc says: 'That is not a cybernetic implant.'\n");
    bionic_name = (string)ob->query_property("bionic_name");
    slot = (string)ob->query_property("cyber_slot");
    if(!bionic_name || !slot) {
        write("Doc says: 'I cannot identify that implant. Wrong vendor type.'\n");
        return 1;
    }
    bdef = (mapping)BIONIC_D->query_bionic_stats(bionic_name);
    if(!mapp(bdef) || !sizeof(bdef)) {
        write("Doc says: 'No surgical protocol on file for that unit.'\n");
        return 1;
    }
    installed = (mapping)BIONIC_D->query_bionics(this_player());
    if(installed[slot]) {
        write("Doc says: 'That slot is already occupied. Remove the old implant first.'\n");
        return 1;
    }
    cost = (int)ob->query_value() / 2;
    if(cost < 500) cost = 500;
    if((int)this_player()->query_money("credits") < cost) {
        write("Doc says: 'Installation fee is " + cost + " credits.'\n");
        return 1;
    }
    this_player()->add_money("credits", -cost);
    if(!BIONIC_D->install_bionic(this_player(), slot, bionic_name)) {
        this_player()->add_money("credits", cost);
        write("Doc says: 'Installation failed. Credits refunded.'\n");
        return 1;
    }
    write("Doc installs the " + (string)ob->query_short() + ". (" + cost + " credits)\n");
    say("The cyber-doc installs a bionic implant on " +
        (string)this_player()->query_cap_name() + ".");
    destruct(ob);
    return 1;
}
