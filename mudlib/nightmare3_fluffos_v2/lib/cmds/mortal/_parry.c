/* /cmds/mortal/_parry.c
   Parry command for Rifts characters.
   Enables autoparry (free parry roll each incoming hit, no APM cost).
   Mirrors the autoparry command for discoverability. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_parry(string str) {
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
    cur = (this_player()->getenv("rifts_autoparry_off") == "1");

    if(lc == "off") {
        this_player()->setenv("rifts_autoparry_off", "1");
        message("my_action",
            "Parry mode OFF. Incoming attacks will not be automatically parried.",
            this_player());
        return 1;
    }

    if(lc == "on" || lc == "") {
        if(!cur) {
            bonus = (int)RIFTS_COMBAT_D->query_parry_bonus(this_player());
            message("my_action",
                "Parry mode is already ON (free 1d20+" + bonus + " parry each attack).",
                this_player());
            return 1;
        }
        this_player()->setenv("rifts_autoparry_off", "0");
        bonus = (int)RIFTS_COMBAT_D->query_parry_bonus(this_player());
        message("my_action",
            "Parry mode ON. Each incoming attack triggers a free parry roll "
            "(1d20+" + bonus + "). Use 'dodge' to switch to dodge mode.",
            this_player());
        return 1;
    }

    notify_fail("Usage: parry [on|off]\n");
    return 0;
}

void help() {
    write(
        "Syntax: parry [on|off]\n\n"
        "Sets your combat mode to parry incoming attacks (default mode).\n\n"
        "  parry / parry on  -- enable free parry rolls (no APM cost)\n"
        "  parry off         -- disable autoparry (attacks land unless dodging)\n\n"
        "Parrying is the default combat stance: each incoming hit triggers a\n"
        "free parry roll (1d20 + bonus) with no action cost.\n\n"
        "See also: dodge, autododge, autoparry, combat\n"
    );
}
