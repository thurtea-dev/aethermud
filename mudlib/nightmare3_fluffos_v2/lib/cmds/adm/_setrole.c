// /cmds/adm/_setrole.c
// Sets wizard position and synced role tag for a wizard player.
// Roles appear in the 'who' display as [Admin-Wiz], [RP-Wiz], etc.

#include <std.h>

inherit DAEMON;

/* Every default whotitle this file or _makewiz.c ever auto-assigns.
   A whotitle matching one of these is a leftover default from an
   earlier rank, not a custom title staff typed in themselves, so a
   later promotion is free to replace it. Anything else is left alone. */
#define AUTO_WHOTITLES ({ \
    "A NEW Apprentice Wizard!", \
    "A NEW Admin Wizard!", \
    "A NEW Domain Wizard!", \
    "A NEW Coding Wizard!", \
    "A NEW Roleplay Wizard!" \
})

private int is_auto_whotitle(string wt) {
    if(!wt || !strlen(wt)) return 1;
    return member_array(wt, AUTO_WHOTITLES) != -1;
}

int cmd_setrole(string str) {
    string pname;
    string role;
    string new_pos;
    string new_title;
    string new_whotitle;
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

    if(role == "admin") {
        new_pos = "arch"; new_title = "Administrator $N";
        new_whotitle = "A NEW Admin Wizard!";
    }
    else if(role == "domain") {
        new_pos = "domain_wiz"; new_title = "Domain Wizard $N";
        new_whotitle = "A NEW Domain Wizard!";
    }
    else if(role == "coding") {
        new_pos = "creator"; new_title = "Coding Wizard $N";
        new_whotitle = "A NEW Coding Wizard!";
    }
    else if(role == "rp") {
        new_pos = "rp_wiz"; new_title = "Roleplay Wizard $N";
        new_whotitle = "A NEW Roleplay Wizard!";
    }
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
    target->setenv("TITLE", new_title);
    /* Default who-list flavor title (separate from TITLE above -- see
       cmds/mortal/_who.c format_wiz_row()). Replaces an unset whotitle
       or a leftover auto-default from an earlier rank (e.g. the
       apprentice default makewiz set), but never a genuine custom
       title staff set themselves. setrole's "admin" role sets position
       "arch", never "head arch", so this never collides with the
       bootstrap founder's hardcoded "The First Admin" display
       fallback. */
    if(is_auto_whotitle((string)target->getenv("whotitle")))
        target->setenv("whotitle", new_whotitle);
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
