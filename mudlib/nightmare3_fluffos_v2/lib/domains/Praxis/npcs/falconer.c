// /domains/Praxis/npcs/falconer.c
// Falconer NPC in monument_square. Trades leather glove for a trained hawk.
// Player must have Falconry skill.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("falconer");
    set_id( ({ "falconer", "man", "woman", "handler", "bird handler" }) );
    set_short("a weathered falconer");
    set_long(
        "A weathered figure in leather vest and gauntlets, a large peregrine\n"
        "hawk perched on one forearm. The falconer's eyes are sharp and\n"
        "patient. A row of caged hawks sits on a low wooden rack nearby.\n"
    );
    set_gender("male");
    set_level(5);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_property("no_steal", 1);
    add_response("hawk",
        "I train and sell hunting hawks. If you have the skill to handle one\n"
        "and bring me a proper leather gauntlet, I can set you up. A trained\n"
        "hawk is worth more than credits. They are partners.\n");
    add_response("glove",
        "A leather falconry glove is the first requirement. The general store\n"
        "carries them. One hundred credits. Bring me one and we will talk.\n");
    add_response("falconry",
        "It is a skill, not a trick. The hawk obeys because it chooses to.\n"
        "You need actual Falconry skill before I trade you a bird.\n");
    add_response("trade",
        "Give me a leather glove and I will match you with one of mine.\n"
        "One hawk per customer. They do not split loyalty.\n");
    set_property("position_str", "tends to a hawk perched on his arm.");
}

private int giver_has_hawk(object giver) {
    object env;
    object *inv;
    int i;

    if(!giver) return 0;
    inv = all_inventory(giver);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i]) continue;
        if((int)inv[i]->query_property("is_hawk")) return 1;
    }
    env = environment(giver);
    if(!env) return 0;
    inv = all_inventory(env);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || inv[i] == giver) continue;
        if((int)inv[i]->query_property("is_hawk") &&
           inv[i]->query_owner() == giver)
            return 1;
    }
    return 0;
}

int receive_object(object ob, object giver) {
    object hawk;
    int skill;

    if(!ob || !giver) return 0;
    if(giver_has_hawk(giver)) {
        tell_object(giver,
            "The falconer holds up a hand. \"You already have a hawk. "
            "One bird per handler.\"\n");
        ob->move(giver);
        return 1;
    }
    if(!(int)ob->query_property("is_leather_glove")) {
        tell_object(giver,
            "The falconer looks at the " + (string)ob->query_short() +
            " and shakes his head. \"I have no use for that.\"\n");
        ob->move(giver);
        return 1;
    }
    skill = (int)RIFTS_SKILLS_D->query_skill_percent(giver, "falconry");
    if(skill <= 0) {
        tell_object(giver,
            "The falconer turns the glove over and hands it back.\n"
            "\"You do not have the Falconry skill. Come back when you do.\"\n");
        ob->move(giver);
        return 1;
    }
    ob->destruct();
    hawk = clone_object("/domains/Praxis/equipment/trained_hawk");
    if(hawk) {
        hawk->set_owner(giver);
        hawk->set_falconry_skill(skill);
        hawk->move(giver);
        tell_object(giver,
            "The falconer nods with approval and hands you a trained hawk.\n"
            "\"She knows basic commands: attack, stay, come, hunt. Say them\n"
            "aloud and she will respond. How well depends on your skill.\"\n");
        giver->tell_room_living(environment(this_object()), giver, 0,
            " receives a trained hawk from the falconer.\n");
    }
    return 1;
}

void reset() { ::reset(); }
