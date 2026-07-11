// /cmds/mortal/_treat.c
// Syntax: treat <target> / bandage <target>
// Rolls First Aid skill. Success: restore 1d6 HP/SDC. Requires bandages or medkit.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private void do_treatment(object player, string target_str) {
    int first_aid;
    int roll;
    int heal;
    object target;
    object bandages;
    int cur_sdc;
    int max_sdc;
    int cur_hp;
    int max_hp;
    int level;

    first_aid = (int)player->query_skill("first aid");
    if(first_aid <= 0) {
        write("You do not have the First Aid skill.\n");
        return;
    }
    if(!target_str || !sizeof(target_str)) {
        write("Treat whom?  Syntax: treat <target>\n");
        return;
    }
    target = present(target_str, environment(player));
    if(!target) {
        write("You do not see " + target_str + " here.\n");
        return;
    }
    if(!living(target)) {
        write("You cannot treat that.\n");
        return;
    }

    // Check for bandages or first aid kit in inventory
    bandages = present("bandages", player);
    if(!bandages) bandages = present("first aid kit", player);
    if(!bandages) bandages = present("medkit", player);
    if(!bandages) {
        write("You need bandages or a first aid kit to treat wounds.\n");
        return;
    }

    level = (int)player->query_level();
    if(level < 1) level = 1;
    first_aid += (level - 1) * 5;
    if(first_aid > 98) first_aid = 98;
    first_aid += (int)ADDICTION_D->query_skill_modifier(player);
    if(first_aid < 1) first_aid = 1;
    roll = random(100) + 1;
    if(roll <= first_aid) {
        heal = random(6) + 1;
        cur_sdc = (int)target->query_stats("SDC");
        max_sdc = (int)target->query_stats("max_SDC");
        if(max_sdc > 0 && cur_sdc < max_sdc) {
            cur_sdc += heal;
            if(cur_sdc > max_sdc) cur_sdc = max_sdc;
            target->set_stats("SDC", cur_sdc);
        } else {
            cur_hp = (int)target->query_stats("rifts_hp");
            max_hp = (int)target->query_stats("max_rifts_hp");
            if(max_hp <= 0) {
                cur_hp = (int)target->query_hp();
                max_hp = (int)target->query_max_hp();
                cur_hp += heal;
                if(cur_hp > max_hp) cur_hp = max_hp;
                target->set_hp(cur_hp);
            } else {
                cur_hp += heal;
                if(cur_hp > max_hp) cur_hp = max_hp;
                target->set_stats("rifts_hp", cur_hp);
            }
        }
        write("You bandage " + (string)target->query_cap_name() +
              "'s wounds. +" + heal + " SDC/HP. "
              "First Aid: " + roll + "/" + first_aid + ".\n");
        tell_object(target,
            (string)player->query_cap_name() +
            " bandages your wounds. +" + heal + " SDC/HP.\n");
    } else {
        write("Your attempt to bandage " + (string)target->query_cap_name() +
              " fails. First Aid: " + roll + "/" + first_aid + ".\n");
    }
}

int cmd_treat(string str) {
    do_treatment(this_player(), str);
    return 1;
}

int cmd_bandage(string str) {
    do_treatment(this_player(), str);
    return 1;
}

void help() {
    write(
        "Syntax: treat <target> / bandage <target>\n\n"
        "Apply first aid to a wounded character.\n"
        "Requires: First Aid skill + bandages or a first aid kit in your inventory.\n"
        "Success: restore 1d6 HP or SDC to the target.\n"
    );
}
