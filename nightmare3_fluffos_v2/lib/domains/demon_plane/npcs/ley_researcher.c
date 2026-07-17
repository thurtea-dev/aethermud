// /domains/demon_plane/npcs/ley_researcher.c
// Field researcher at the Praxis dimensional tear. Quest giver for demon_sliver_shard.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("ley researcher");
    set_npc_name("the ley researcher");
    set_id(({ "ley researcher", "researcher", "field researcher", "scientist" }));
    set_short("a ley line researcher");
    set_long(
        "A wiry scientist in scorched field gear, goggles pushed up on their\n"
        "forehead. Sensor leads run from a belt pack to the tripod beside the\n"
        "dimensional tear. They look like they have not slept since the rift opened.");
    set_level(4);
    set_max_hp(50);
    set_hp(50);
    set_aggressive(0);
    set_moving(0);
    set_gender(random(2) ? "male" : "female");
    set_race("human");
    set_alignment(100);
    set_body_type("human");

    add_response("hello",
        "the ley researcher says: Keep back from the tear unless you know ley physics.");
    add_response("tear",
        "the ley researcher says: That wound leads to a demon-plane sliver. I need a shard sample from the far side.");
    add_response("rift",
        "the ley researcher says: Step through if you must. Bring back a stable rift shard amulet from their shrine.");
    add_response("work", "");
    add_response("job", "");
    add_response("quest", "");
    add_response("shard", "");
    add_response("amulet", "");
    set_default_response(
        "The researcher mutters over a flickering sensor display.");
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says: %s", a, b) != 2) return;
    b = lower_case(b);

    if(strsrch(b, "work") != -1 || strsrch(b, "job") != -1 ||
       strsrch(b, "quest") != -1) {
        tp = this_player();
        if(tp) call_out("offer_shard_quest", 1, tp);
        return;
    }

    if(strsrch(b, "turn in") != -1 || strsrch(b, "deliver") != -1 ||
       strsrch(b, "sample") != -1 || strsrch(b, "here") != -1) {
        tp = this_player();
        if(tp) call_out("try_turn_in_shard", 1, tp);
    }
}

void offer_shard_quest(object tp) {
    if(!tp || !objectp(tp)) return;

    if((int)QUEST_D->is_quest_complete(tp, "demon_sliver_shard")) {
        tell_object(tp,
            "The researcher says: I already have enough shard data. Thank you.\n");
        return;
    }
    if((int)QUEST_D->is_quest_active(tp, "demon_sliver_shard")) {
        tell_object(tp,
            "The researcher says: Find the rift shrine inside the sliver and bring me the amulet.\n");
        return;
    }
    tell_object(tp,
        "The researcher says: I will pay for a rift shard amulet from the demon-plane shrine. "
        "Step through the tear, reach the Bone Corridor north of the wastes, and take the sample from the altar. "
        "Say 'work' again if you accept.\n");
    catch(QUEST_D->start_quest(tp, "demon_sliver_shard"));
}

void try_turn_in_shard(object tp) {
    object *inv;
    object amulet;
    int i;
    int completed;

    if(!tp || !objectp(tp)) return;
    if(!(int)QUEST_D->is_quest_active(tp, "demon_sliver_shard")) {
        tell_object(tp,
            "The researcher says: I only buy samples for the active shard study.\n");
        return;
    }

    inv = all_inventory(tp);
    amulet = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i]) continue;
        if((string)inv[i]->query_name() == "rift shard amulet") {
            amulet = inv[i];
            break;
        }
    }
    if(!amulet) {
        tell_object(tp,
            "The researcher says: I need the rift shard amulet from the demon-plane shrine.\n");
        return;
    }

    amulet->move(this_object());
    destruct(amulet);
    catch(QUEST_D->advance_quest(tp, "demon_sliver_shard", "rift_shard_amulet"));
    completed = (int)QUEST_D->complete_quest(tp, "demon_sliver_shard");
    if(completed)
        tell_object(tp,
            "The researcher seals the amulet in a lead case, grinning despite the danger.\n");
}
