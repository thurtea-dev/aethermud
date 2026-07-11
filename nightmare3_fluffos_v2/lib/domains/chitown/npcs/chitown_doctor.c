// /domains/chitown/npcs/chitown_doctor.c
// Chi-Town medical clinic doctor. Heals HP/SDC or MDC for 400cr.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("chitown doctor");
    set_npc_name("Doctor");
    set_id( ({ "chitown doctor", "doctor", "medic", "physician", "doc" }) );
    set_short("the clinic doctor");
    set_long(
        "A Coalition medical officer in a clean white coat. The doctor moves\n"
        "with practiced efficiency, checking patients and reviewing readouts.\n"
        "\nSay 'heal me' for treatment (400 credits). Also sells first aid kits.");
    set_level(5);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(300);
    set_body_type("human");

    add_response("hello", "Doctor says: 'Welcome to the clinic. Say heal me for treatment.'");
    add_response("hi", "Doctor says: 'Welcome to the clinic. Say heal me for treatment.'");
    add_response("heal", "");
    add_response("buy", "Doctor says: 'Type buy kit for a first aid kit at 200 credits.'");
    add_response("bye", "Doctor says: 'Stay healthy out there.'");
    set_default_response("The doctor glances up briefly then returns to work.");
}

void init() {
    ::init();
    add_action("cmd_heal", "heal");
    add_action("cmd_buy", "buy");
}

void catch_tell(string str) {
    string a;
    string b;
    object tp;

    if(sscanf(str, "%s says: %s", a, b) != 2) {
        ::catch_tell(str);
        return;
    }
    b = lower_case(b);
    tp = this_player();
    if(!tp) { ::catch_tell(str); return; }
    if(strsrch(b, "heal") != -1 || strsrch(b, "fix") != -1 ||
       strsrch(b, "hurt") != -1) {
        call_out("do_heal", 1, tp);
        return;
    }
    ::catch_tell(str);
}

void do_heal(object tp) {
    int is_mdc;
    int max_mdc;
    int max_hp;
    int max_sdc;

    if(!tp || !objectp(tp)) return;
    if((int)tp->query_money("credits") < 400) {
        say_here("Treatment costs 400 credits. You don't have enough.");
        return;
    }
    tp->add_money("credits", -400);
    is_mdc = (int)tp->query_stats("is_MDC");
    max_hp = (int)tp->query_stats("max_rifts_hp");
    max_sdc = (int)tp->query_stats("max_SDC");
    max_mdc = (int)tp->query_stats("max_MDC");

    if(is_mdc) {
        if(max_mdc > 0) {
            tp->set_stats("MDC", max_mdc > 50 ? 50 : max_mdc);
        }
        tell_object(tp,
            "The doctor patches your armor seals and applies repair compound. "
            "+50 MDC restored.\n");
    } else {
        if(max_hp > 0) tp->set_stats("rifts_hp", max_hp);
        if(max_sdc > 0) tp->set_stats("SDC", max_sdc);
        tell_object(tp,
            "The doctor treats your wounds with practiced speed. "
            "HP and SDC fully restored.\n");
    }
    tell_room(environment(this_object()),
        "The doctor treats " + tp->query_cap_name() + "'s wounds.\n",
        ({ this_object(), tp }));
}

int cmd_heal(string str) {
    if(!str || lower_case(str) != "me") return 0;
    do_heal(this_player());
    return 1;
}

int cmd_buy(string str) {
    object ob;

    if(!str || lower_case(str) != "kit") {
        write("Buy what? Try: buy kit\n");
        return 1;
    }
    if((int)this_player()->query_money("credits") < 200) {
        say_here("A first aid kit costs 200 credits. You don't have enough.");
        return 1;
    }
    ob = clone_object(EQ_PATH+"first_aid_kit");
    if(!ob) {
        say_here("Out of first aid kits at the moment.");
        return 1;
    }
    this_player()->add_money("credits", -200);
    if(ob->move(this_player())) {
        write("You cannot carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay 200 credits. The doctor hands you a first aid kit.\n");
    return 1;
}
