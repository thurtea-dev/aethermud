/* /cmds/mortal/_pemote.c
   Possessive emote. Prepends the character's name with "'s" before the text.
   Example: pemote sword glows. -> "Bob's sword glows." */

#include <std.h>

inherit DAEMON;

#define SZ 75

int cmd_pemote(string str) {
    object env;
    object *inv;
    string line;
    int i;

    if((int)this_player()->query_ghost()) {
        notify_fail("A ghost has no voice.\n");
        return 0;
    }
    env = environment(this_player());
    if(!str) {
        write("Pemote what?\n");
        return 1;
    }
    if((int)this_player()->query_mp() < 0 &&
       (int)this_player()->query_level() < 20) {
        write("You are low on magic points.\n");
        return 1;
    }
    if((int)this_player()->query_level() < 20)
        this_player()->add_mp(-7);
    write(sprintf("%-=" + SZ + "s",
        "You pemote: " + this_player()->query_cap_name() + "'s " + str));
    inv = all_inventory(env);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i]) || inv[i] == this_player()) continue;
        line = sprintf("%-=" + SZ + "s",
            (string)this_player()->query_display_name(inv[i]) + "'s " + str);
        tell_object(inv[i], line);
    }
    return 1;
}
