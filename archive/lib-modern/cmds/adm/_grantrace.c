/* /cmds/adm/_grantrace.c
   Admin command: backfill race spells and psionics for a named player.
   Use for characters created before the race-grant fix was applied. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_grantrace(string str) {
    string pname;
    string race;
    string spells_before;
    string psionics_before;
    string spells_after;
    string psionics_after;
    object target;

    if(!creatorp(this_player())) {
        notify_fail("Admin access required.\n");
        return 0;
    }
    if(!str || !strlen(str)) {
        write("Usage: grantrace <playername>\n");
        return 1;
    }
    pname = lower_case(str);
    target = find_player(pname);
    if(!target || !objectp(target)) {
        write("Player '" + capitalize(pname) + "' is not online.\n");
        return 1;
    }
    race = (string)target->query_race();
    if(!race || !strlen(race)) {
        write((string)target->query_cap_name() + " has no race set.\n");
        return 1;
    }

    spells_before   = (string)target->getenv("known_spells");
    psionics_before = (string)target->getenv("known_psionics");
    if(!spells_before)   spells_before   = "";
    if(!psionics_before) psionics_before = "";

    catch(("/daemon/rifts_start_d")->grant_race_package(target));

    spells_after   = (string)target->getenv("known_spells");
    psionics_after = (string)target->getenv("known_psionics");
    if(!spells_after)   spells_after   = "";
    if(!psionics_after) psionics_after = "";

    write("Race grant applied to " + (string)target->query_cap_name() +
          " (" + race + ").\n");
    if(spells_after != spells_before)
        write("  Spells now: " + spells_after + "\n");
    else
        write("  Spells: no change.\n");
    if(psionics_after != psionics_before)
        write("  Psionics now: " + psionics_after + "\n");
    else
        write("  Psionics: no change.\n");
    write("  PPE: " + (int)target->query_stats("PPE") + "/" +
          (int)target->query_stats("max_PPE") + "\n");
    write("  ISP: " + (int)target->query_stats("ISP") + "/" +
          (int)target->query_stats("max_ISP") + "\n");

    tell_object(target,
        "An administrator has updated your racial abilities.\n");

    catch(target->save_player(pname));

    log_file("/log/adm/grantrace",
        ctime(time()) + " " + (string)this_player()->query_name() +
        " granted race package to " + pname + " (" + race + ")\n");
    return 1;
}

void help() {
    write(
        "Syntax: grantrace <playername>\n\n"
        "Grants race-based spells and psionics to a player.\n"
        "Use for characters created before the race-grant fix.\n"
        "Does not re-give equipment or OCC skills.\n"
        "Player must be online. Admin only.\n\n"
        "Example: grantrace playername\n"
    );
}
