/* /cmds/adm/_setrcc.c
   Set a player's race (RCC). */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_setrcc(string str) {
    string name, race;
    object target;
    string *rflags;
    int sdc, mdc, hp, ppe, isp;
    mapping rolls;
    string *kk;
    int i, tmp;

    if(!creatorp(this_player())) {
        write("Access denied.\n");
        return 1;
    }
    if(!str || sscanf(str, "%s %s", name, race) != 2) {
        write("Syntax: setrcc <player> <race_name>\n"
              "Example: setrcc bob human\n");
        return 1;
    }
    name   = lower_case(name);
    race   = lower_case(race);
    target = find_player(name);
    if(!target) {
        write("Player '" + capitalize(name) + "' is not online.\n");
        return 1;
    }
    if(!(int)RIFTS_D->is_rifts_race(race) &&
       !(int)RIFTS_D->is_admin_race(race)) {
        write("'" + race + "' is not a valid Rifts race.\n");
        return 1;
    }

    target->set_race(race);
    target->new_body();

    /* Re-roll stats for the new race. */
    rolls = (mapping)RIFTS_D->do_rifts_rolls(race);
    kk = keys(rolls);
    tmp = sizeof(kk);
    for(i = 0; i < tmp; i++)
        target->set_stats(kk[i], rolls[kk[i]]);

    /* Re-init damage pools. */
    if((int)RIFTS_D->is_mdc_race(race)) {
        mdc = (int)RIFTS_D->init_mdc(race, rolls["PE"]);
        target->set_stats("MDC",     mdc);
        target->set_stats("max_MDC", mdc);
        target->set_stats("SDC",     0);
        target->set_stats("max_SDC", 0);
        target->set_stats("is_MDC",  1);
    } else {
        sdc = (int)RIFTS_D->init_sdc(race, rolls["PE"]);
        target->set_stats("SDC",     sdc);
        target->set_stats("max_SDC", sdc);
        target->set_stats("MDC",     0);
        target->set_stats("max_MDC", 0);
        target->set_stats("is_MDC",  0);
    }
    hp = (int)RIFTS_D->init_hp(race, rolls["PE"]);
    target->set_stats("rifts_hp",     hp);
    target->set_stats("max_rifts_hp", hp);

    rflags = (string *)RIFTS_D->query_race_flags(race);
    target->setenv("rifts_flags", implode(rflags, ","));

    write("Set " + capitalize(name) + "'s race to: " + capitalize(race) + "\n"
          "Stats re-rolled and damage pools re-initialized.\n");
    tell_object(target,
        "%^YELLOW%^Your race has been changed to " + capitalize(race) +
        " by " + (string)this_player()->query_cap_name() +
        ".%^RESET%^\n");
    return 1;
}

void help() {
    write("Syntax: setrcc <player> <race_name>\n\n"
          "Sets a player's race and re-rolls attributes for that race.\n"
          "Damage pools (MDC/SDC/HP) are re-initialized.\n"
          "Player must be online. Race must be a valid Rifts race.\n"
          "Example: setrcc bob elf\n"
          "Restricted to wizards.\n");
}
