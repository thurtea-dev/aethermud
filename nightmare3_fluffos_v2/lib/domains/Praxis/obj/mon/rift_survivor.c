// /d/Praxis/obj/mon/rift_survivor.c
// Guide NPC for new Rifts Earth arrivals.
// catch_tell matches on keywords anywhere in the say string.

#include <std.h>

inherit MONSTER;

void say_line(string str);

void create() {
    ::create();
    set_name("survivor");
    set_id( ({ "survivor", "guide", "woman", "scout" }) );
    set_short("a weathered survivor");
    set_long(
        "A lean woman in patched Coalition-surplus gear, a battered laser "
        "rifle slung over one shoulder. She has the hollow eyes of someone "
        "who has watched the Rifts swallow cities whole. A hand-lettered "
        "patch on her shoulder reads PRAXIS SCOUT.\n"
        "\nShe looks like she will answer your questions. "
        "Try: say hello");
    set_level(5);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(300);
    set_body_type("human");
    set_property("position_str", "looks around nervously.");
}

void init() {
    if(this_player() && !creatorp(this_player()) &&
       living(this_player()) && interactive(this_player())) {
        remove_call_out("greet_arrival");
        call_out("greet_arrival", 2);
    }
}

void greet_arrival() {
    if(!environment(this_object())) return;
    tell_room(environment(this_object()),
        "The survivor glances over. \"Fresh out of a Rift. "
        "You look confused. Say 'help' if you need a rundown.\"\n",
        ({ this_object() }) );
}

void catch_tell(string str) {
    string low;

    if(strsrch(str, " says") == -1) return;
    low = lower_case(str);

    if(strsrch(low, "hello") != -1 || strsrch(low, " hi ") != -1 ||
       strsrch(low, "\"hi\"") != -1) {
        call_out("say_line", 1,
            "The survivor nods. \"Another soul the Rifts spat out. "
            "Stay alert. Ask me about 'help', 'south', or 'praxis' "
            "if you need to get your bearings.\"");
        return;
    }

    if(strsrch(low, "help") != -1 || strsrch(low, "command") != -1 ||
       strsrch(low, "guide") != -1 || strsrch(low, "new") != -1 ||
       strsrch(low, "start") != -1) {
        call_out("say_line", 1,
            "The survivor says: Here is what you need to know.\n"
            " look examine your surroundings\n"
            " inventory see what you carry\n"
            " score your character stats\n"
            " who players currently online\n"
            " help <topic> in-game help files\n"
            " north/south/east/west move between areas\n"
            " say <text> speak aloud\n"
            " kill <target> initiate combat\n"
            " quests track jobs you accept\n"
            " language see your known languages\n"
            " The settlement of Praxis is to the SOUTH.");
        return;
    }

    if(strsrch(low, "south") != -1) {
        call_out("say_line", 1,
            "The survivor points south. \"Monument Square. "
            "That is the heart of Praxis. Vendors, travelers, "
            "and the occasional mercenary. Safer than out here.\"");
        return;
    }

    if(strsrch(low, "praxis") != -1) {
        call_out("say_line", 1,
            "The survivor says: Praxis is what we call this place. "
            "Nobody knows what it was before the Rifts. "
            "Now it is a settlement. Humans, D-Bees, even the occasional "
            "Coalition deserter. Not paradise, but it is alive.");
        return;
    }
}

void say_line(string str) {
    if(!environment(this_object())) return;
    tell_room(environment(this_object()), str + "\n", ({ this_object() }) );
}
