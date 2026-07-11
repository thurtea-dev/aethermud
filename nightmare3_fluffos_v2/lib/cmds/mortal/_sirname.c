// /cmds/mortal/_sirname.c
// Removed: sirname system replaced by RP-Wizard granted rp_title system.

#include <std.h>

inherit DAEMON;

int cmd_sirname(string str) {
    write("The sirname system has been removed.\n"
          "Titles are now granted by RP-Wizards.  "
          "See: help request title\n");
    return 1;
}
