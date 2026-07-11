/* /domains/Praxis/obj/mon/veteran_arbiter.c
   Level 15 repeatable life quest: bring a bandit patch as proof of survival. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("arbiter");
    set_npc_name("Arbiter Kane");
    set_id(({ "arbiter", "kane", "veteran", "arbiter kane", "officer",
              "coordinator" }));
    set_short("Arbiter Kane, Coalition survival coordinator");
    set_long(
        "A hard-faced Coalition veteran in faded dress grays, one sleeve\n"
        "pinned at the shoulder where an old wound never healed clean. He\n"
        "keeps a ledger of proven survivors and stamps each entry with a\n"
        "wax seal bearing the CS skull. His voice is flat and practiced,\n"
        "the tone of someone who has heard every excuse and believed none.\n"
        "\nHe grants extra lives to operators who prove they can survive the\n"
        "roads. Level fifteen minimum. Bring a bandit patch from the old\n"
        "highway and give it to him.");
    set_level(8);
    set_max_hp(100);
    set_hp(100);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(200);
    set_body_type("human");

    add_response("hello",
        "Arbiter Kane says: Level fifteen or higher. Bring proof from the highway bandits.");
    add_response("hi",
        "Arbiter Kane says: Level fifteen or higher. Bring proof from the highway bandits.");
    add_response("quest",
        "Arbiter Kane says: Kill a highway bandit north of the checkpoint. "
        "Bring me the raider patch. Survive the roads, earn an extra life.");
    add_response("life",
        "Arbiter Kane says: One extra life per proven survival run. "
        "Bandit patch required. Twenty-four hours between claims.");
    add_response("lives",
        "Arbiter Kane says: One extra life per proven survival run. "
        "Bandit patch required. Twenty-four hours between claims.");
    add_response("proof",
        "Arbiter Kane says: A bandit patch from the old highway. "
        "Give it to me when you have it.");
    add_response("patch",
        "Arbiter Kane says: Give me the bandit patch when you are ready.");
    add_response("bandit",
        "Arbiter Kane says: Highway bandits prowl the ruins north of the checkpoint.");
    add_response("bye",
        "Arbiter Kane says: Stay alive. That is the whole job.");
    set_property("position_str", "reviews a ledger of survivor records.");
    set_default_response("Arbiter Kane waits for proof, not promises.");
}

int receive_object(object ob, object giver) {
    string ob_name;
    int level;
    int cooldown;
    int completed;

    if(!ob || !giver || !living(giver)) return 0;
    ob_name = lower_case((string)ob->query_name());

    if(strsrch(ob_name, "patch") == -1 && strsrch(ob_name, "bandit") == -1) {
        tell_object(giver,
            "Arbiter Kane shakes his head. \"Not what I asked for.\"\n");
        ob->move(giver);
        return 1;
    }

    level = (int)giver->query_level();
    if(level < 15) {
        tell_object(giver,
            "Arbiter Kane says: \"Come back when you have seen enough road. "
            "Level fifteen minimum.\"\n");
        ob->move(giver);
        return 1;
    }

    cooldown = (int)QUEST_D->query_quest_cooldown(giver, "survival_proof");
    if(cooldown > 0) {
        tell_object(giver,
            "Arbiter Kane says: \"You already proved yourself. "
            "Give it a day before you ask again.\"\n");
        ob->move(giver);
        return 1;
    }

    if(!(int)QUEST_D->is_quest_active(giver, "survival_proof")) {
        if(!(int)QUEST_D->start_quest(giver, "survival_proof")) {
            ob->move(giver);
            return 1;
        }
    }
    catch(QUEST_D->advance_quest(giver, "survival_proof", "bandit_patch"));
    completed = (int)QUEST_D->complete_quest(giver, "survival_proof");
    if(!completed) {
        tell_object(giver,
            "Arbiter Kane frowns at his ledger. \"Something went wrong. Try again.\"\n");
        ob->move(giver);
        return 1;
    }

    ob->destruct();

    tell_object(giver,
        "Arbiter Kane takes the patch and stamps your record.\n"
        "\"Survived the roads. That earns an extra life in my book.\"\n");
    tell_room(environment(this_object()),
        "Arbiter Kane records something in a ledger and nods at " +
        (string)giver->query_cap_name() + ".\n",
        ({ this_object(), giver }));
    return 1;
}
