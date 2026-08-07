/* /secure/cmds/creator/_wizz.c
   Teleport the calling wizard to the Wizard's Lounge. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_wizz(string a) {
    object dest;

    if(ambassadorp(previous_object())) return 0;
    if(!creatorp(this_player())) {
        write("This command is for wizard staff only.\n");
        return 1;
    }
    if(environment(this_player()) == find_object("/domains/wizards/lounge")) {
        write("You are already in the lounge.\n");
        return 1;
    }
    dest = find_object("/domains/wizards/lounge");
    if(!dest) dest = load_object("/domains/wizards/lounge");
    if(!objectp(dest)) {
        write("The lounge could not be loaded.\n");
        return 1;
    }
    this_player()->move_player("/domains/wizards/lounge");
    return 1;
}

int help() {
    write("Syntax: wizz\n" +
          "Teleports you to the Wizard's Lounge from anywhere.\n");
    return 1;
}
