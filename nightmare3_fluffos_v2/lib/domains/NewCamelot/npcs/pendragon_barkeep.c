// /domains/NewCamelot/npcs/pendragon_barkeep.c
// Pendragon Inn barkeep. Sells drinks, shares rumors.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

private string *__rumors;
private int __rumor_index;

void create() {
    ::create();

    set_name("pendragon barkeep");
    set_npc_name("Barkeep");
    set_id( ({ "pendragon barkeep", "barkeep", "bartender", "innkeeper", "keeper" }) );
    set_short("the Pendragon Inn barkeep");
    set_long(
        "A broad-shouldered woman with close-cropped gray hair and calm,\n"
        "watchful eyes. She moves behind the bar with the efficiency of\n"
        "long practice. A towel over one shoulder, a glass in hand.\n"
        "\nAsk about 'drink' or 'rumors'. Drinks cost 30 credits.");
    set_level(4);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(300);
    set_body_type("human");

    add_response("hello", "");
    add_response("hi", "");
    add_response("drink", "");
    add_response("ale", "");
    add_response("rumor", "");
    add_response("rumours", "");
    add_response("news", "");
    add_response("bard", "Barkeep says: 'The bard is Celene. She travels the old roads. Always has a story.'");
    add_response("lazlo", "Barkeep says: 'Good folk up north. We trade with them when we can.'");
    add_response("bye", "Barkeep says: 'Stay warm. The nights get cold here.'");
    set_default_response("The barkeep polishes a glass and waits.");

    __rumors = ({
        "A dragon was spotted near the Rhine last week.",
        "A ley line nexus tore open somewhere in the Black Forest. People are going missing.",
        "There is talk of a Cyber-Knight tournament being organized near the old Camelot ruins."
    });
    __rumor_index = 0;
}

void init() {
    ::init();
    add_action("cmd_drink", "drink");
    add_action("cmd_drink", "buy");
    add_action("cmd_rumor", "rumor");
    add_action("cmd_rumor", "rumours");
    add_action("cmd_rumor", "news");
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

    if(strsrch(b, "hello") != -1 || strsrch(b, "hi") != -1) {
        say_here("Welcome to the Pendragon. What can I get you?");
        return;
    }
    if(strsrch(b, "drink") != -1 || strsrch(b, "ale") != -1 ||
       strsrch(b, "beer") != -1) {
        call_out("do_drink", 1, tp);
        return;
    }
    if(strsrch(b, "rumor") != -1 || strsrch(b, "rumour") != -1 ||
       strsrch(b, "news") != -1) {
        call_out("do_rumor", 1, tp);
        return;
    }
    if(strsrch(b, "found it") != -1 || strsrch(b, "found") != -1) {
        call_out("check_forest_quest_done", 1, tp);
        return;
    }
    if(strsrch(b, "forest") != -1 || strsrch(b, "black forest") != -1) {
        call_out("offer_forest_quest", 1, tp);
        return;
    }
    ::catch_tell(str);
}

void do_drink(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)tp->query_money("credits") < 30) {
        say_here("A drink is 30 credits. Come back when you're flush.");
        return;
    }
    tp->add_money("credits", -30);
    tp->set_stats("SDC",
        (int)tp->query_stats("SDC") + 5);
    tell_object(tp,
        "The barkeep sets a pewter tankard in front of you. Dark, slightly bitter,\n"
        "with a faint herbal aftertaste. It settles warmly. (+5 SDC)\n");
    tell_room(environment(this_object()),
        "The barkeep pours " + tp->query_cap_name() + " a drink.\n",
        ({ this_object(), tp }));
}

void do_rumor(object tp) {
    string rumor;

    if(!tp || !objectp(tp)) return;
    rumor = __rumors[__rumor_index % sizeof(__rumors)];
    __rumor_index++;
    say_here("Heard this one yesterday: '" + rumor + "'");
    if(strsrch(lower_case(rumor), "black forest") != -1) {
        call_out("offer_forest_quest", 2, tp);
    }
}

void offer_forest_quest(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "black_forest_nexus") ||
       (int)QUEST_D->is_quest_active(tp, "black_forest_nexus")) return;
    say_here("If you are heading that way, I would pay well for information. "
             "Bring me something from the nexus and I will make it worth your time. "
             "North of Europe Road, into the Black Forest.");
    catch(QUEST_D->start_quest(tp, "black_forest_nexus"));
}

void check_forest_quest_done(object tp) {
    object stone;

    if(!tp || !objectp(tp)) return;
    if(!(int)QUEST_D->is_quest_active(tp, "black_forest_nexus")) {
        say_here("I have nothing for you right now.");
        return;
    }
    if(!(int)QUEST_D->is_objective_complete(tp, "black_forest_nexus", "nexus_stone_found")) {
        say_here("Bring me the nexus stone from the Black Forest. North of Europe Road.");
        return;
    }
    stone = present("nexus stone", tp);
    if(!stone) {
        say_here("Where is the stone? You said you found it.");
        return;
    }
    stone->set_property("quest_item", 0);
    if(!(int)QUEST_D->complete_quest(tp, "black_forest_nexus")) {
        say_here("Something went wrong turning in the stone. Try again.");
        return;
    }
    catch(FACTION_D->add_rep(tp, "new_camelot", 50));
    catch(FACTION_D->add_rep(tp, "lazlo", 50));
    say_here("Excellent work. This will help us understand what is happening up there. Well done.");
}

int cmd_drink(string str) {
    if(str && lower_case(str) != "drink" && lower_case(str) != "ale" &&
       lower_case(str) != "beer" && lower_case(str) != "ale") {
        if(strsrch(lower_case(str), "drink") == -1 &&
           strsrch(lower_case(str), "ale") == -1) return 0;
    }
    do_drink(this_player());
    return 1;
}

int cmd_rumor(string str) {
    do_rumor(this_player());
    return 1;
}
