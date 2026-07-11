// /cmds/mortal/_quests.c
// quests [completed | all | <id>]  -- track active and completed quests.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private void write_survival_status(object player) {
    mapping qdef;
    int cooldown;
    int level;
    int hours;

    qdef = (mapping)QUEST_D->query_quest("survival_proof");
    if(!qdef) return;
    level = (int)player->query_level();
    cooldown = (int)QUEST_D->query_quest_cooldown(player, "survival_proof");
    write("\nRepeatable quest:\n");
    write("  " + (string)qdef["name"] + "  [survival_proof]\n");
    if(level < 15)
        write("    Requires level 15. Bring a bandit patch to Arbiter Kane.\n");
    else if(cooldown > 0) {
        hours = (cooldown + 3599) / 3600;
        if(hours == 1)
            write("    Cooldown: about 1 hour remaining.\n");
        else
            write("    Cooldown: about " + hours + " hours remaining.\n");
    } else
        write("    Available. Bring a bandit patch to Arbiter Kane at the hospital.\n");
}

void init() {
    add_action("cmd_quests", "quests");
}

int cmd_quests(string str) {
    mapping aq, quest_def, prog, objs, all_q;
    string *cq, *aq_keys, *obj_keys, *all_ids;
    string s, hint, qstatus;
    int i, j, req, cur;
    int is_secret, is_active, is_done, is_repeatable, cooldown;

    if(!str || !strlen(str)) {
        aq = (mapping)QUEST_D->query_active_quests(this_player());
        aq_keys = keys(aq);
        if(!sizeof(aq_keys)) {
            write("You have no active quests.  Talk to people around Monument Square.\n");
            write_survival_status(this_player());
            return 1;
        }
        write("Active quests:\n");
        i = sizeof(aq_keys);
        while(i--) {
            quest_def = (mapping)QUEST_D->query_quest(aq_keys[i]);
            if(!quest_def) continue;
            prog  = (mapping)aq[aq_keys[i]];
            objs  = (mapping)quest_def["objectives"];
            hint  = (string)quest_def["start_location"];
            write("  " + (string)quest_def["name"] + "  [" + aq_keys[i] + "]\n");
            if(hint && sizeof(hint))
                write("    Hint: " + hint + "\n");
            if(objs) {
                obj_keys = keys(objs);
                j = sizeof(obj_keys);
                while(j--) {
                    req = (int)objs[obj_keys[j]];
                    cur = mapp(prog) ? (int)prog[obj_keys[j]] : 0;
                    write("    " + obj_keys[j] + ": " + cur + "/" + req +
                        (cur >= req ? " [done]" : "") + "\n");
                }
            }
        }
        write("Type 'quests <id>' for details.  'quests completed' or 'quests all'.\n");
        write_survival_status(this_player());
        return 1;
    }

    s = lower_case(str);

    if(s == "completed") {
        cq = (string *)QUEST_D->query_completed_quests(this_player());
        if(!sizeof(cq)) {
            write("You have not completed any quests yet.\n");
            return 1;
        }
        write("Completed quests:\n");
        i = sizeof(cq);
        while(i--) {
            quest_def = (mapping)QUEST_D->query_quest(cq[i]);
            if(quest_def)
                write("  " + (string)quest_def["name"] + "\n");
            else
                write("  " + cq[i] + "\n");
        }
        return 1;
    }

    if(s == "all") {
        all_q   = (mapping)QUEST_D->query_all_quests();
        if(!all_q || !sizeof(all_q)) {
            write("No quests defined.\n");
            return 1;
        }
        all_ids = keys(all_q);
        aq      = (mapping)QUEST_D->query_active_quests(this_player());
        cq      = (string *)QUEST_D->query_completed_quests(this_player());

        write("All quests:\n");
        i = sizeof(all_ids);
        while(i--) {
            quest_def = all_q[all_ids[i]];
            if(!quest_def) continue;
            is_secret = (int)quest_def["secret"];
            is_active = (aq && mapp(aq[all_ids[i]]));
            is_done   = (member_array(all_ids[i], cq) != -1);
            is_repeatable = (int)quest_def["repeatable"];
            /* hide secret quests the player has not encountered */
            if(is_secret && !is_active && !is_done) continue;
            hint   = (string)quest_def["start_location"];
            qstatus = "";
            if(is_active)    qstatus = " [active]";
            else if(is_done) qstatus = " [done]";
            else if(is_repeatable) {
                cooldown = (int)QUEST_D->query_quest_cooldown(this_player(), all_ids[i]);
                if(cooldown > 0)
                    qstatus = " [cooldown]";
                else
                    qstatus = " [available]";
            }
            write("  " + (string)quest_def["name"] + qstatus + "\n");
            if(hint && sizeof(hint))
                write("    " + hint + "\n");
        }
        return 1;
    }

    quest_def = (mapping)QUEST_D->query_quest(s);
    if(!quest_def) {
        write("No quest found with id: " + str + "\n");
        return 1;
    }
    aq = (mapping)QUEST_D->query_active_quests(this_player());
    cq = (string *)QUEST_D->query_completed_quests(this_player());
    /* secret quests appear not to exist unless the player has started them */
    if((int)quest_def["secret"] && !aq[s] &&
       member_array(s, cq) == -1) {
        write("No quest found with id: " + str + "\n");
        return 1;
    }
    if(!aq[s]) {
        write("You do not have that quest active.\n");
        return 1;
    }
    write("Quest: " + (string)quest_def["name"] + "\n");
    write((string)quest_def["desc"] + "\n");
    hint = (string)quest_def["start_location"];
    if(hint && sizeof(hint))
        write("Hint: " + hint + "\n");
    prog = (mapping)aq[s];
    objs = (mapping)quest_def["objectives"];
    if(objs) {
        write("Objectives:\n");
        obj_keys = keys(objs);
        j = sizeof(obj_keys);
        while(j--) {
            req = (int)objs[obj_keys[j]];
            cur = mapp(prog) ? (int)prog[obj_keys[j]] : 0;
            write("  " + obj_keys[j] + ": " + cur + "/" + req +
                (cur >= req ? " [done]" : "") + "\n");
        }
    }
    return 1;
}
