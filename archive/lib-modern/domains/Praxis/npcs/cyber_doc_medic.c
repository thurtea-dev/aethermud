// /domains/Praxis/npcs/cyber_doc_medic.c
// Cyber-doc surgeon for bionic installation.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("cyber-doc");
    set_npc_name("Doc");
    set_id(({ "cyber-doc", "cyber doc", "doc", "surgeon", "doctor" }));
    set_short("a cyber-doc surgeon");
    set_long(
        "A gaunt surgeon in blood-spotted scrubs with chrome-plated fingers.\n"
        "Implant diagrams cover the walls. Type 'install <cybernetic>' while\n"
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
    add_response("help", "Doc says: 'Install <item> while you carry a cybernetic implant.'");
    set_default_response("The cyber-doc adjusts a laser scalpel and waits.");
}

void init() {
    ::init();
    add_action("cmd_install", "install");
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
