/* /cmds/mortal/_greet.c
   Introduce-lite: a polite greeting that also runs the introduction system. */

#include <std.h>

inherit DAEMON;

private void do_greet_one(object who, object target) {
    string wname;
    string tname;

    if(!who || !target || who == target) return;
    if(!userp(target)) return;
    wname = (string)who->query_cap_name();
    tname = (string)target->query_cap_name();
    if(!(int)who->knows_player((string)target->query_name()) ||
       !(int)target->knows_player((string)who->query_name()))
        who->force_me("introduce " + (string)target->query_name());
    tell_room(environment(who),
        wname + " greets " + tname + ".",
        ({ who, target }));
    write("You greet " + tname + ".\n");
    tell_object(target, wname + " greets you.\n");
}

int cmd_greet(string str) {
    object *inv;
    object target;
    object who;
    int i;
    int count;

    who = this_player();
    if(!str || !sizeof(str)) {
        inv = all_inventory(environment(who));
        count = 0;
        for(i = 0; i < sizeof(inv); i++) {
            if(inv[i] == who) continue;
            if(!living(inv[i]) || !userp(inv[i])) continue;
            do_greet_one(who, inv[i]);
            count++;
        }
        if(!count)
            write("There is no one here to greet.\n");
        return 1;
    }
    target = present(str, environment(who));
    if(!target || !living(target)) {
        write("You don't see " + str + " here.\n");
        return 1;
    }
    if(!userp(target)) {
        write("You can only greet other players that way.\n");
        return 1;
    }
    do_greet_one(who, target);
    return 1;
}

void help() {
    write(
        "Syntax: greet\n"
        "        greet <player>\n\n"
        "Offers a polite greeting. If you have not been introduced yet,\n"
        "greet also runs the introduction system so both parties learn\n"
        "each other's names.\n\n"
        "Without an argument, greets every other player in the room.\n\n"
        "See also: introduce, say, emote\n"
    );
}
