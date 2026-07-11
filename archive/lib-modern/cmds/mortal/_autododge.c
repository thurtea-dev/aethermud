// /cmds/mortal/_autododge.c
// RiftsMUD  -  toggle autododge on/off for Rifts characters.
// When enabled, the character spends 1 APM per incoming attack to dodge
// instead of using the free autoparry.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_autododge(string str) {
    string lc;
    string race;
    int cur;
    int dodge;

    race = (string)this_player()->query_race();
    if(!(int)RIFTS_D->is_rifts_race(race)) {
        notify_fail("That is a Rifts combat option.\n");
        return 0;
    }

    lc  = str ? lower_case(str) : "";
    cur = (this_player()->getenv("rifts_autododge") == "1");

    if(lc == "on") {
        this_player()->setenv("rifts_autododge", "1");
        dodge = (int)RIFTS_COMBAT_D->query_dodge_bonus(this_player());
        message("my_action",
            "Autododge enabled. You will spend 1 APM to dodge each incoming attack "
            "(1d20+" + dodge + ").",
            this_player());
        return 1;
    }

    if(lc == "off") {
        this_player()->setenv("rifts_autododge", "0");
        message("my_action",
            "Autododge disabled. You will autoparry incoming attacks for free.",
            this_player());
        return 1;
    }

    // Toggle with no argument
    if(cur) {
        this_player()->setenv("rifts_autododge", "0");
        message("my_action",
            "Autododge OFF. You will now autoparry incoming attacks for free.",
            this_player());
    } else {
        this_player()->setenv("rifts_autododge", "1");
        dodge = (int)RIFTS_COMBAT_D->query_dodge_bonus(this_player());
        message("my_action",
            "Autododge ON. You will spend 1 APM to dodge each incoming attack "
            "(1d20+" + dodge + ").",
            this_player());
    }
    return 1;
}

void help() {
    message("help",
        "Syntax: autododge [on|off]\n\n"
        "Toggles automatic dodging on or off for Rifts characters.\n\n"
        "When OFF (default): you autoparry each attack for free using your\n"
        "  parry bonus (1d20 + parry bonus vs attacker's strike roll).\n\n"
        "When ON: you spend 1 APM (attack per melee) to dodge each incoming\n"
        "  attack. Costs an action but uses your dodge bonus.\n\n"
        "See also: autoparry, score",
        this_player());
}
