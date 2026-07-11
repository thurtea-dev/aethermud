/* /cmds/adm/_setskill.c
   Set a Rifts skill percentage on an online player. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_setskill(string str) {
    string name, skill;
    int pct;
    object target;

    if(!creatorp(this_player())) {
        write("Access denied.\n");
        return 1;
    }
    if(!str || sscanf(str, "%s %s %d", name, skill, pct) != 3) {
        write("Syntax: setskill <player> <skill_name> <percentage>\n"
              "Example: setskill bob \"wp energy rifle\" 65\n");
        return 1;
    }
    name  = lower_case(name);
    skill = lower_case(skill);
    target = find_player(name);
    if(!target) {
        write("Player '" + capitalize(name) + "' is not online.\n");
        return 1;
    }
    if(pct < 0)   pct = 0;
    if(pct > 100) pct = 100;
    RIFTS_SKILLS_D->grant_skill(target, skill, pct);
    write("Set " + capitalize(name) + "'s " + skill + " to " + pct + "%.\n");
    tell_object(target,
        "%^YELLOW%^Your " + capitalize(skill) +
        " skill has been set to " + pct + "%.%^RESET%^\n");
    return 1;
}

void help() {
    write("Syntax: setskill <player> <skill> <percentage>\n\n"
          "Set a named Rifts skill on an online player. Percentage 0-100.\n"
          "Example: setskill bob tracking 55\n"
          "Restricted to wizards.\n");
}
