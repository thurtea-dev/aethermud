// /secure/cmds/adm/_grant.c
// Admin: grant title <player> <title>
// Requires creator rank or above.

#include <std.h>
#include <dirs.h>

inherit DAEMON;

#define TITLE_GRANT_LOG "title_grants"

int cmd_grant(string str) {
    string rest;
    string pname;
    string title;
    object target;
    string logline;
    int i;

    if(!creatorp(this_player())) {
        notify_fail("You don't have permission to grant titles.\n");
        return 0;
    }
    if(!str || str == "" || !sscanf(str, "title %s", rest)) {
        write("Usage: grant title <player> <title>\n");
        return 1;
    }
    i = strsrch(rest, " ");
    if(i < 1) {
        write("Usage: grant title <player> <title>\n");
        return 1;
    }
    pname = lower_case(rest[0..i-1]);
    title = capitalize(rest[i+1..]);

    if(strlen(title) < 2 || strlen(title) > 20) {
        write("Title must be between 2 and 20 characters.\n");
        return 1;
    }
    target = find_player(pname);
    if(!target) {
        write("Player '" + pname + "' is not online.\n");
        return 1;
    }
    target->set_rp_title(title);
    write("You grant the title '" + title + "' to " + capitalize(pname) + ".\n");
    tell_object(target,
        "%^YELLOW%^You have been granted the title '" + title + "' by " +
        (string)this_player()->query_cap_name() + ".%^RESET%^\n");
    logline = ctime(time()) + " " + (string)this_player()->query_name() +
              " granted '" + title + "' to " + pname + "\n";
    log_file(TITLE_GRANT_LOG, logline);
    return 1;
}

void help() {
    write(
        "Syntax: grant title <player> <title>\n\n"
        "Grants an RP title prefix to an online player.\n"
        "  Example: grant title playername Lord\n\n"
        "The title appears before the player's first name for anyone introduced to them.\n"
        "Title must be letters/spaces only, 2-20 characters.  Requires creator rank.\n\n"
        "See also: revoke title, request title\n"
    );
}
