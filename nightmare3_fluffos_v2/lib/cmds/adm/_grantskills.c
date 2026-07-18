/* /cmds/adm/_grantskills.c
   Admin command: backfill OCC skills for a named player.
   Use for characters created before the skill-grant fix, or after an OCC change. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_grantskills(string str) {
    string pname;
    string occ;
    string *skills_before;
    string *skills_after;
    int count_before;
    int count_after;
    object target;

    if(!creatorp(this_player())) {
        notify_fail("Admin access required.\n");
        return 0;
    }
    if(!str || !strlen(str)) {
        write("Usage: grantskills <playername>\n");
        return 1;
    }
    pname = lower_case(str);
    target = find_player(pname);
    if(!target || !objectp(target)) {
        write("Player '" + capitalize(pname) + "' is not online.\n");
        return 1;
    }
    occ = (string)target->getenv("rifts_occ");
    if(!occ || !strlen(occ)) occ = "none";

    skills_before = (string *)RIFTS_SKILLS_D->query_player_skills(target);
    if(!skills_before) skills_before = ({});
    count_before = sizeof(skills_before);

    catch(RIFTS_START_D->grant_occ_skills(target));

    skills_after = (string *)RIFTS_SKILLS_D->query_player_skills(target);
    if(!skills_after) skills_after = ({});
    count_after = sizeof(skills_after);

    write("OCC skill grant applied to " + (string)target->query_cap_name() +
          " (OCC: " + occ + ").\n");
    write("  Skills before: " + count_before + "\n");
    write("  Skills after:  " + count_after  + "\n");
    write("  Skills added:  " + (count_after - count_before) + "\n");

    tell_object(target,
        "An administrator has updated your OCC skills.\n");

    catch(target->save_player(pname));

    log_file("adm/grantskills",
        ctime(time()) + " " + (string)this_player()->query_name() +
        " granted OCC skills to " + pname +
        " (OCC: " + occ + ")\n");
    return 1;
}

void init() {
    add_action("cmd_grantskills", "grantskills");
}

void help() {
    write(
        "Syntax: grantskills <playername>\n\n"
        "Grants OCC-based skills to a player using their stored OCC and race.\n"
        "Use for characters created before the skill-grant fix, or after an OCC change.\n"
        "Player must be online. Admin only.\n\n"
        "Example: grantskills playername\n"
    );
}
