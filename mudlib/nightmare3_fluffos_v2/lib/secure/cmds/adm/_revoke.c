// /secure/cmds/adm/_revoke.c
// Admin: revoke title <player>
// Requires creator rank or above.

#include <std.h>
#include <dirs.h>

inherit DAEMON;

#define TITLE_GRANT_LOG "title_grants"

int cmd_revoke(string str) {
    string pname;
    object target;
    string logline;

    if(!creatorp(this_player())) {
        notify_fail("You don't have permission to revoke titles.\n");
        return 0;
    }
    if(!str || str == "" || !sscanf(str, "title %s", pname)) {
        write("Usage: revoke title <player>\n");
        return 1;
    }
    pname = lower_case(pname);
    target = find_player(pname);
    if(!target) {
        write("Player '" + pname + "' is not online.\n");
        return 1;
    }
    if(!(string)target->query_rp_title() || !strlen((string)target->query_rp_title())) {
        write(capitalize(pname) + " does not have a title to revoke.\n");
        return 1;
    }
    target->set_rp_title(0);
    write("Title revoked from " + capitalize(pname) + ".\n");
    tell_object(target,
        "%^YELLOW%^Your title has been revoked by " +
        (string)this_player()->query_cap_name() + ".%^RESET%^\n");
    logline = ctime(time()) + " " + (string)this_player()->query_name() +
              " revoked title from " + pname + "\n";
    log_file(TITLE_GRANT_LOG, logline);
    return 1;
}

void help() {
    write(
        "Syntax: revoke title <player>\n\n"
        "Removes the RP title prefix from an online player.\n"
        "  Example: revoke title playername\n\n"
        "Requires creator rank.\n\n"
        "See also: grant title, request title\n"
    );
}
