/* /cmds/mortal/_forward.c
   Toggle mail forwarding to another player. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_forward(string str) {
    string target;

    if(!str || !sizeof(str)) {
        target = (string)MAIL_D->query_forward((string)this_player()->query_name());
        if(!target || !sizeof(target))
            write("Mail forwarding is off.\n");
        else
            write("Your mail is forwarded to " + capitalize(target) + ".\n");
        return 1;
    }
    if(lower_case(str) == "off" || lower_case(str) == "none") {
        MAIL_D->set_forward((string)this_player()->query_name(), "");
        write("Mail forwarding disabled.\n");
        return 1;
    }
    target = lower_case(str);
    if(target == lower_case((string)this_player()->query_name())) {
        write("You cannot forward mail to yourself.\n");
        return 1;
    }
    MAIL_D->set_forward((string)this_player()->query_name(), target);
    write("Incoming mail will also be copied to " + capitalize(target) + ".\n");
    return 1;
}

void help() {
    write(
        "Syntax: forward\n"
        "        forward <player>\n"
        "        forward off\n\n"
        "Copies incoming mail to another player. The original still\n"
        "arrives in your mailbox. Use forward off to disable.\n\n"
        "See also: mail\n"
    );
}
