// /cmds/mortal/_autoparry.c
// AetherMUD  -  toggle autoparry on/off.
// When enabled (default), defender autoparries each incoming attack for free.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_autoparry(string str) {
    string lc;
    string race;
    int cur;
    int parry;

    race = (string)this_player()->query_race();
    if(!(int)RIFTS_D->is_rifts_race(race)) {
        notify_fail("That is a Rifts combat option.\n");
        return 0;
    }

    lc = str ? lower_case(str) : "";

    if(lc == "on") {
        this_player()->setenv("rifts_autoparry_off", "0");
        parry = (int)RIFTS_COMBAT_D->query_parry_bonus(this_player());
        message("my_action",
            "Autoparry enabled. Each incoming attack triggers a free parry roll "
            "(1d20+" + parry + ").",
            this_player());
        return 1;
    }

    if(lc == "off") {
        this_player()->setenv("rifts_autoparry_off", "1");
        message("my_action",
            "Autoparry disabled. Incoming attacks will land unless you autododge.",
            this_player());
        return 1;
    }

    // Status / no argument
    cur   = (this_player()->getenv("rifts_autoparry_off") == "1");
    parry = (int)RIFTS_COMBAT_D->query_parry_bonus(this_player());
    message("my_action",
        "Autoparry is currently " + (!cur ? "ON" : "OFF") + ".  "
        "Parry bonus: 1d20+" + parry + ".\n"
        "Use: autoparry on  /  autoparry off",
        this_player());
    return 1;
}

void help() {
    message("help",
        "Syntax: autoparry [on|off]\n\n"
        "Toggles automatic parrying. When ON (default): each incoming attack\n"
        "triggers a free parry roll (1d20 + parry bonus) with no APM cost.\n"
        "When OFF: incoming attacks land unless you have autododge enabled.\n\n"
        "See also: autododge, score",
        this_player());
}
