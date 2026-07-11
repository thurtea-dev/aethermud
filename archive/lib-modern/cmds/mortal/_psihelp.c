/* /cmds/mortal/_psihelp.c
   Shortcut: psihelp [topic] -> help psionics or help <topic> */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_psihelp(string str) {
    string topic;

    topic = (str && sizeof(str)) ? lower_case(str) : "psionics";
    HELP_D->cmd_help(topic, 0, 0);
    return 1;
}

void help() {
    write("Syntax: psihelp [topic]\n"
          "Shows psionic help. Type psihelp alone for the full psionics overview.\n"
          "Type psihelp psi for command syntax.\n"
          "Type psihelp psisword for the psi-sword.\n");
}
