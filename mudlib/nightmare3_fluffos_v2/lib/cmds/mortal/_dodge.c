/* /cmds/mortal/_dodge.c
   Dodge command for Rifts characters.
   Toggles autododge on (spending APM to dodge the next incoming attack)
   rather than autoparrying for free.  Mirrors autododge for discoverability. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_dodge(string str) {
    string lc;
    string race;
    int cur;
    int bonus;

    race = (string)this_player()->query_race();
    if(!(int)RIFTS_D->is_rifts_race(race)) {
        notify_fail("That is a Rifts combat option.\n");
        return 0;
    }

    lc  = str ? lower_case(str) : "";
    cur = (this_player()->getenv("rifts_autododge") == "1");

    if(lc == "off") {
        this_player()->setenv("rifts_autododge", "0");
        message("my_action",
            "Dodge mode OFF. You will autoparry incoming attacks for free.",
            this_player());
        return 1;
    }

    if(lc == "on" || lc == "") {
        if(cur) {
            bonus = (int)RIFTS_COMBAT_D->query_dodge_bonus(this_player());
            message("my_action",
                "Dodge mode is already ON (1d20+" + bonus + " to dodge each attack).",
                this_player());
            return 1;
        }
        this_player()->setenv("rifts_autododge", "1");
        bonus = (int)RIFTS_COMBAT_D->query_dodge_bonus(this_player());
        message("my_action",
            "Dodge mode ON. You will spend 1 APM to dodge each incoming attack "
            "(1d20+" + bonus + "). Use 'parry' to switch back.",
            this_player());
        return 1;
    }

    notify_fail("Usage: dodge [on|off]\n");
    return 0;
}

void help() {
    write(
        "Syntax: dodge [on|off]\n\n"
        "Sets your combat mode to dodge incoming attacks instead of parrying.\n\n"
        "  dodge / dodge on  -- spend 1 APM per hit to attempt a dodge roll\n"
        "  dodge off         -- return to autoparry (free, costs no APM)\n\n"
        "In Rifts combat, dodging costs one attack per melee but uses your\n"
        "full dodge bonus (1d20 + bonus). Parrying is free but lower.\n\n"
        "See also: parry, autododge, autoparry, combat\n"
    );
}
