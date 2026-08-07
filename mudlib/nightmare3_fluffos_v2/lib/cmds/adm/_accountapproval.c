// /cmds/adm/_accountapproval.c
// Lists and approves accounts pending staff approval before their
// first character can finish chargen. See secure/daemon/account_d.c
// (pending_approval field) and secure/std/login.c (check_pending_approval()).

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_accountapproval(string str) {
    string cmd, name;
    string *pending;
    string email;
    int i;

    if(!admin_wizp(this_player())) {
        notify_fail("You don't have permission to review account approvals.\n");
        return 0;
    }
    if(!str || !sizeof(str)) {
        write("Usage: accountapproval list\n"
              "       accountapproval approve <account>\n");
        return 1;
    }
    if(sscanf(str, "%s %s", cmd, name) != 2) {
        cmd = str;
        name = 0;
    }
    cmd = lower_case(cmd);

    if(cmd == "list") {
        pending = (string *)ACCOUNT_D->list_pending_accounts();
        if(!pending || !sizeof(pending)) {
            write("No accounts are pending approval.\n");
            return 1;
        }
        write("=== Accounts Pending Approval (" + sizeof(pending) + ") ===\n");
        for(i = 0; i < sizeof(pending); i++) {
            email = (string)ACCOUNT_D->query_account_email(pending[i]);
            write(sprintf("  %-16s  %s\n", capitalize(pending[i]),
                (email && sizeof(email)) ? email : "(no email given)"));
        }
        write("Use 'accountapproval approve <account>' to approve one.\n");
        return 1;
    }

    if(cmd == "approve") {
        if(!name || !sizeof(name)) {
            write("Usage: accountapproval approve <account>\n");
            return 1;
        }
        name = lower_case(name);
        if(!(int)ACCOUNT_D->account_exists(name)) {
            write("No such account: " + capitalize(name) + "\n");
            return 1;
        }
        if(!(int)ACCOUNT_D->is_pending_approval(name)) {
            write(capitalize(name) + " is not pending approval.\n");
            return 1;
        }
        ACCOUNT_D->approve_account(name);
        write("Approved " + capitalize(name) +
              ". They may now log in and finish character creation.\n");
        log_file("adm/accountapproval",
            ctime(time()) + " " + (string)this_player()->query_name() +
            " approved account " + name + "\n");
        return 1;
    }

    write("Usage: accountapproval list\n"
          "       accountapproval approve <account>\n");
    return 1;
}

void help() {
    write(
        "Syntax: accountapproval list\n"
        "        accountapproval approve <account>\n\n"
        "Reviews accounts held pending staff approval. A brand-new account\n"
        "(created after this MUD already has an admin) cannot finish\n"
        "character creation until approved here; the player sees a\n"
        "'pending approval' message on login until then.\n\n"
        "  accountapproval list             -- Show accounts awaiting approval.\n"
        "  accountapproval approve <name>   -- Approve one; logged to\n"
        "                                       /log/adm/accountapproval.\n\n"
        "Restricted to admin wizards.\n\n"
        "See also: setrole\n"
    );
}
