/* /cmds/adm/_sever.c
   Admin/RP tool for the narrative limb-severing system (2026-07-19).
   Limbs no longer take damage; severing is a story-facing state used
   for cybernetics, veteran scars, and special effects.

   Syntax:
     sever <player> <limb>            - sever a limb
     sever <player> restore <limb>    - restore a severed limb
     sever <player> list              - list limbs and severed limbs   */

#include <std.h>

inherit DAEMON;

int cmd_sever(string str) {
    string pname, rest, limb;
    string *limbs, *missing;
    object target;
    int i;

    if(!admin_wizp(this_player())) {
        notify_fail("You don't have permission to do that.\n");
        return 0;
    }
    if(!str || sscanf(str, "%s %s", pname, rest) != 2) {
        write("Usage: sever <player> <limb>\n"
              "       sever <player> restore <limb>\n"
              "       sever <player> list\n");
        return 1;
    }
    target = find_player(lower_case(pname));
    if(!target) {
        write("Player '" + capitalize(pname) + "' is not online.\n");
        return 1;
    }
    if(rest == "list") {
        limbs = (string *)target->query_limbs();
        missing = (string *)target->query_severed_limbs();
        write("Limbs: " + ((limbs && sizeof(limbs)) ?
            implode(limbs - ({ "whole_body" }), ", ") : "(none)"));
        write("Severed: " + ((missing && sizeof(missing)) ?
            implode(missing, ", ") : "(none)"));
        return 1;
    }
    if(sscanf(rest, "restore %s", limb) == 1) {
        if(!(int)target->restore_limb(limb)) {
            write("Cannot restore '" + limb + "' on " +
                (string)target->query_cap_name() +
                " (not severed, or already present).\n");
            return 1;
        }
        write("Restored " + limb + " on " +
            (string)target->query_cap_name() + ".\n");
        tell_object(target, "Your " + limb + " has been restored!\n");
        log_file("adm/sever", (string)this_player()->query_name() +
            " restored " + limb + " on " + (string)target->query_name() +
            " " + ctime(time()) + "\n");
        return 1;
    }
    limb = rest;
    if(!(int)target->sever_limb(limb)) {
        write("Cannot sever '" + limb + "' on " +
            (string)target->query_cap_name() +
            " (no such limb, or it is not severable).\n");
        return 1;
    }
    write("Severed " + limb + " on " +
        (string)target->query_cap_name() + ".\n");
    log_file("adm/sever", (string)this_player()->query_name() +
        " severed " + limb + " on " + (string)target->query_name() +
        " " + ctime(time()) + "\n");
    return 1;
}

void help() {
    write("Syntax: sever <player> <limb>\n"
          "        sever <player> restore <limb>\n"
          "        sever <player> list\n\n"
          "Narratively severs or restores a limb on a player for RP\n"
          "purposes (cybernetic replacement stories, veteran scars,\n"
          "special effects). Severing has no damage interaction: all\n"
          "combat damage is pooled on the body. Severed limbs show in\n"
          "look, body, and limbs output, and can also be restored by\n"
          "the cleric replace skill.\n");
}
