/* /cmds/mortal/_commands.c
   List player commands in columns. */

#include <std.h>
#include <daemons.h>
#include <dirs.h>

inherit DAEMON;

#define CMD_COLUMNS 5

int cmd_commands(string str) {
    string *cmds;

    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }
    if(str && sizeof(str)) return 0;

    cmds = sort_array(
        (string *)CMD_D->query_commands(DIR_MORTAL_CMDS) +
        (string *)CMD_D->query_commands(DIR_SECURE_MORTAL_CMDS),
        1);
    write("=== PLAYER COMMANDS ===");
    write(format_page(cmds, CMD_COLUMNS));
    write("");
    write("Type help <command> for details on any command above.");
    return 1;
}

void help() {
    write("Syntax: commands\n\n"
          "Lists available player commands in five columns.\n"
          "See also: help, syntax\n");
}
