/* /cmds/adm/_setfaction.c
   Admin command to set or clear a player's faction membership.
   Syntax:
     setfaction <player> <faction>
     setfaction <player> none        -- remove from faction */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_setfaction(string str) {
    string pname, faction, old_faction, rank;
    object target, fd;
    int sp;

    if(!archp(this_player()) &&
       (string)this_player()->getenv("wiz_role") != "admin") {
        notify_fail("Permission denied.\n");
        return 0;
    }

    if(!str || !strlen(str)) {
        write("Usage: setfaction <player> <faction|none>\n");
        return 1;
    }

    sp = strsrch(str, " ");
    if(sp == -1) {
        write("Usage: setfaction <player> <faction|none>\n");
        return 1;
    }

    pname   = lower_case(str[0..sp-1]);
    faction = lower_case(str[sp+1..]);

    target = find_player(pname);
    if(!target || !objectp(target)) {
        write("Player '" + capitalize(pname) + "' is not online.\n");
        return 1;
    }

    fd = find_object(FACTION_D);
    if(!fd) fd = load_object(FACTION_D);
    if(!fd) {
        write("Faction daemon unavailable.\n");
        return 1;
    }

    old_faction = (string)fd->query_faction(pname);

    if(faction == "none") {
        if(!old_faction || old_faction == "") {
            write(capitalize(pname) + " is not in any faction.\n");
            return 1;
        }
        (int)fd->remove_member(old_faction, pname);
        tell_object(target,
            capitalize(pname) + " has been removed from faction '" +
            old_faction + "' by " +
            (string)this_player()->query_cap_name() + ".\n");
        write("Removed " + capitalize(pname) + " from faction '" +
              old_faction + "'.\n");
        log_file("adm/setfaction",
            ctime(time()) + " " + (string)this_player()->query_name() +
            " removed " + pname + " from " + old_faction + "\n");
        return 1;
    }

    if(!mapp((mapping)fd->query_faction_data(faction))) {
        write("Unknown faction: " + faction + "\n"
              "Use 'factions' command or see help setfaction for the list.\n");
        return 1;
    }

    if(old_faction && old_faction != "") {
        (int)fd->remove_member(old_faction, pname);
    }

    rank = "member";
    (int)fd->add_member(faction, pname, rank);

    write("Set " + capitalize(pname) + " faction to '" + faction + "'.\n");
    tell_object(target,
        "Your faction has been set to '" + faction + "' by " +
        (string)this_player()->query_cap_name() + ".\n");
    log_file("adm/setfaction",
        ctime(time()) + " " + (string)this_player()->query_name() +
        " set " + pname + " to " + faction + "\n");
    return 1;
}

void help() {
    write(
        "Syntax: setfaction <player> <faction>\n"
        "        setfaction <player> none\n\n"
        "Sets or clears a player's faction membership.\n\n"
        "Factions include: coalition, ngr, triax, lazlo, tolkeen,\n"
        "splugorth, atlantean_nomads (and others).\n\n"
        "Secret factions (aerihman, sunaj) exist but are not listed publicly.\n\n"
        "Restricted to admin-level wizards.\n\n"
        "Log: /log/adm/setfaction\n"
        "See also: faction_d\n"
    );
}
