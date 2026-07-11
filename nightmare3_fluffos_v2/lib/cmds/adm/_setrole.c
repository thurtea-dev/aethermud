// /cmds/adm/_setrole.c
// Sets wizard position and synced role tag for a wizard player.
// Roles appear in the 'who' display as [Admin-Wiz], [RP-Wiz], etc.

#include <std.h>

inherit DAEMON;

int cmd_setrole(string str) {
    string pname;
    string role;
    string new_pos;
    object target;

    if(!admin_wizp(this_player())) {
        notify_fail("You don't have permission to set wizard roles.\n");
        return 0;
    }
    if(!str || sscanf(str, "%s %s", pname, role) != 2) {
        write("Usage: setrole <player> <admin|domain|coding|rp>\n");
        return 1;
    }
    pname = lower_case(pname);
    role = lower_case(role);

    if(role == "admin") new_pos = "arch";
    else if(role == "domain") new_pos = "domain_wiz";
    else if(role == "coding") new_pos = "creator";
    else if(role == "rp") new_pos = "rp_wiz";
    else {
        write("Valid roles: admin, domain, coding, rp\n");
        return 1;
    }
    target = find_player(pname);
    if(!target) {
        write("Player '" + capitalize(pname) + "' is not online.\n");
        return 1;
    }
    if(!creatorp(target)) {
        write(capitalize(pname) + " is not a wizard.\n");
        return 1;
    }
    target->set_position(new_pos);
    target->save_player((string)target->query_name());
    write("Set " + (string)target->query_cap_name() + "'s wizard role to '" +
          role + "'.\n");
    tell_object(target,
        "%^YELLOW%^Your wizard role has been set to '" + role +
        "' by " + (string)this_player()->query_cap_name() + ".%^RESET%^\n");
    log_file("adm/setrole",
        ctime(time()) + " " + (string)this_player()->query_name() +
        " set role of " + pname + " to " + role + " (" + new_pos + ")\n");
    return 1;
}

void help() {
    write(
        "Syntax: setrole <player> <admin|domain|coding|rp>\n\n"
        "Sets a wizard's position and synced role tag.\n\n"
        "  admin   -> arch         [Admin-Wiz]\n"
        "  domain  -> domain_wiz   [Domain-Wiz]\n"
        "  coding  -> creator      [Coding-Wiz]\n"
        "  rp      -> rp_wiz       [RP-Wiz]\n\n"
        "The target player must be online and must already be a wizard.\n"
        "To promote a mortal to Apprentice first, use: makewiz <player>\n\n"
        "Typical sequence:\n"
        "  makewiz splynncryth\n"
        "  (apprentice completes tasks)\n"
        "  setrole splynncryth admin\n\n"
        "Position and role are saved to the player file immediately.\n"
        "Restricted to admin wizards.\n\n"
        "Example: setrole playername admin\n"
        "See also: makewiz, who\n"
    );
}
