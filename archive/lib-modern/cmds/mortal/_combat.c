/* /cmds/mortal/_combat.c
   Display Rifts combat statistics for the current player. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_combat(string str) {
    string race;
    string occ;
    string hth;
    string enemy;
    string autodg;
    string autopry;
    int apm;
    int strike;
    int parry;
    int dodge;
    int damage;
    int init_bonus;
    object attacker;

    race = (string)this_player()->query_race();
    if(!(int)RIFTS_D->is_rifts_race(race)) {
        write("Combat statistics are for Rifts characters.\n");
        return 1;
    }

    occ       = (string)this_player()->getenv("rifts_occ");
    hth       = (string)this_player()->getenv("rifts_hth");
    apm       = (int)RIFTS_COMBAT_D->query_apm(this_player());
    strike    = (int)RIFTS_COMBAT_D->query_strike_bonus(this_player());
    parry     = (int)RIFTS_COMBAT_D->query_parry_bonus(this_player());
    dodge     = (int)RIFTS_COMBAT_D->query_dodge_bonus(this_player());
    damage    = (int)RIFTS_COMBAT_D->query_damage_bonus(this_player());
    init_bonus = (int)RIFTS_COMBAT_D->query_initiative_bonus(this_player());

    if(!hth || hth == "") hth = "Basic";
    if(!occ || occ == "") occ = "None";

    autodg  = (this_player()->getenv("rifts_autododge") == "1") ? "on" : "off";
    autopry = (this_player()->getenv("rifts_autoparry_off") != "1") ? "on" : "off";

    attacker = (object)this_player()->query_current_attacker();
    if(attacker && objectp(attacker))
        enemy = (string)attacker->query_cap_name();
    else
        enemy = "none";

    write("Combat Statistics:\n");
    write("  H2H Style:   " + capitalize(hth) + "\n");
    write("  APM:         " + apm + " attacks per melee round\n");
    write("  Initiative:  1d20+" + init_bonus + "\n");
    write("  Strike:      1d20+" + strike + "\n");
    write("  Parry:       1d20+" + parry + "\n");
    write("  Dodge:       1d20+" + dodge + "\n");
    if(damage > 0)
        write("  Damage Bonus:+" + damage + "\n");
    write("  Auto-parry:  " + autopry + "\n");
    write("  Auto-dodge:  " + autodg + "\n");
    write("  Target:      " + enemy + "\n");
    write("Use 'autododge on/off' and 'autoparry on/off' to change mode.\n");
    return 1;
}

void help() {
    write(
        "Syntax: combat\n\n"
        "Displays your Rifts combat statistics:\n"
        "  H2H style, APM, initiative, strike, parry, dodge, and damage bonuses.\n"
        "  Also shows current autododge/autoparry mode and current fight target.\n\n"
        "See also: autododge, autoparry, score, kill\n"
    );
}
