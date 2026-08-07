// /cmds/mortal/_introduce.c
// Mutual introduction: both parties learn each other's name.
// Usage:
//   introduce
//   introduce <player>
//   introduce yourself to <player>

#include <std.h>

inherit DAEMON;

private void do_introduction(object who, object target) {
    who->add_to_face_list((string)target->query_name());
    target->add_to_face_list((string)who->query_name());

    write("You introduce yourself to " + (string)target->query_cap_name() + ".\n");
    tell_object(target,
        (string)who->query_cap_name() +
        " introduces themselves to you.\n");
    tell_room(environment(who),
        (string)who->query_cap_name() +
        " introduces themselves to " +
        (string)target->query_cap_name() + ".",
        ({ who, target })
    );
}

private int already_introduced(object who, object target) {
    return (int)who->knows_player((string)target->query_name()) &&
           (int)target->knows_player((string)who->query_name());
}

int cmd_introduce(string str) {
    object *inv;
    object target;
    object who;
    string tname;
    int i;
    int count;

    who = this_player();

    if(!str || str == "") {
        inv = all_inventory(environment(who));
        for(i = 0, count = 0; i < sizeof(inv); i++) {
            if(inv[i] == who) continue;
            if(!living(inv[i]) || !userp(inv[i])) continue;

            if(already_introduced(who, inv[i])) continue;

            target = inv[i];
            do_introduction(who, target);
            count++;
        }

        if(!count) {
            write("There is no one here to introduce yourself to.\n");
        }
        return 1;
    }

    if(!sscanf(str, "yourself to %s", tname))
        tname = str;

    if(!tname || !strlen(tname)) {
        write("Introduce yourself to whom? Type: introduce <name>\n");
        return 1;
    }

    if(tname == "yourself") {
        write("You already know yourself.\n");
        return 1;
    }

    target = present(tname, environment(who));
    if(!target || !living(target)) {
        write("You don't see " + tname + " here.\n");
        return 1;
    }
    if(target == who) {
        write("You already know yourself.\n");
        return 1;
    }
    if(!userp(target)) {
        write("You can only introduce yourself to other players.\n");
        return 1;
    }
    if(already_introduced(who, target)) {
        write("You have already been introduced to " +
              (string)target->query_cap_name() + ".\n");
        return 1;
    }

    do_introduction(who, target);
    return 1;
}

void help() {
    write(
        "Syntax: introduce\n"
        "        introduce <player>\n"
        "        introduce yourself to <player>\n\n"
        "Without an argument, introduces you to every other player in the room.\n"
        "With a name, introduces you to that one player.\n"
        "Introduction is mutual: both parties learn each other's name.\n"
    );
}