/* /cmds/mortal/_roll.c
   Syntax: roll <skill name>
   Rolls d100 against the player's skill value. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_roll(string str) {
    string skill_name;
    int skill_val;
    int roll_result;
    int level;
    string step;

    step = (string)this_player()->getenv("creation_step");
    if(step && step != "" && step != "done") return 0;
    if(!str || !sizeof(str)) {
        write("Roll which skill?  Syntax: roll <skill name>\n");
        return 1;
    }
    skill_name = lower_case(str);
    skill_val  = (int)this_player()->query_skill(skill_name);
    if(skill_val <= 0) {
        write("You do not have the skill: " + str + "\n");
        return 1;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    skill_val += (level - 1) * 5;
    if(skill_val > 98) skill_val = 98;
    skill_val += (int)ADDICTION_D->query_skill_modifier(this_player());
    if(skill_val < 1) skill_val = 1;
    roll_result = random(100) + 1;
    if(roll_result <= skill_val) {
        write("SUCCESS. " + capitalize(skill_name) + " check: " +
              roll_result + "/" + skill_val + ".\n");
    } else {
        write("FAILURE. " + capitalize(skill_name) + " check: " +
              roll_result + "/" + skill_val + ".\n");
    }
    return 1;
}

void help() {
    write(
        "Syntax: roll <skill name>\n\n"
        "Roll d100 against your skill value.\n"
        "  Success: roll <= skill value\n"
        "  Failure: roll > skill value\n\n"
        "Examples:\n"
        "  roll prowl\n"
        "  roll first aid\n"
        "  roll tracking\n"
    );
}
