// /cmds/mortal/_sneak.c
// Syntax: sneak / hide
// Rolls Prowl skill. Success sets is_sneaking=1 so room entry is silent.
// Broken by combat, loud commands, or being in a lit area without cover.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private void do_attempt(object player) {
    int prowl;
    int roll;
    int level;

    prowl = (int)player->query_skill("prowl");
    if(prowl <= 0) {
        write("You do not have the Prowl skill.\n");
        return;
    }
    if((int)player->query_property("is_sneaking")) {
        write("You are already moving silently.\n");
        return;
    }
    level = (int)player->query_level();
    if(level < 1) level = 1;
    prowl += (level - 1) * 5;
    if(prowl > 98) prowl = 98;
    prowl += (int)ADDICTION_D->query_skill_modifier(player);
    if(prowl < 1) prowl = 1;
    roll = random(100) + 1;
    if(roll <= prowl) {
        player->set_property("is_sneaking", 1);
        write("You move silently, blending into the shadows. "
              "Prowl check: " + roll + "/" + prowl + ".\n");
    } else {
        write("You fail to move silently. "
              "Prowl check: " + roll + "/" + prowl + ".\n");
    }
}

int cmd_sneak(string str) {
    do_attempt(this_player());
    return 1;
}

int cmd_hide(string str) {
    do_attempt(this_player());
    return 1;
}

void help() {
    write(
        "Syntax: sneak / hide\n\n"
        "Attempt to move silently using the Prowl skill.\n"
        "Success: you move undetected. Others do not see your room entry messages.\n"
        "Failure: you make too much noise.\n"
        "Broken by: entering combat, shouting, or bright unobstructed rooms.\n"
    );
}
