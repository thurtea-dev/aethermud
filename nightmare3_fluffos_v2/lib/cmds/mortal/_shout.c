// /cmds/mortal/_shout.c
// Room-local shout command. No language filtering -- everyone in the
// room understands. 5-second cooldown between shouts.

#include <std.h>

inherit DAEMON;

int cmd_shout(string str) {
    int last_shout;
    string last_str;

    if(!str || str == "") {
        notify_fail("Shout what?\n");
        return 0;
    }
    if(strlen(str) > 200) {
        notify_fail("Your shout is too long.\n");
        return 0;
    }
    if(this_player()->query_ghost()) {
        message("my_action", "You moan mournfully.", this_player());
        return 1;
    }

    last_str = (string)this_player()->getenv("last_shout_time");
    last_shout = last_str ? to_int(last_str) : 0;
    if((time() - last_shout) < 5) {
        notify_fail("You are still catching your breath from shouting.\n");
        return 0;
    }

    if((int)this_player()->query_property("is_sneaking")) {
        this_player()->remove_property("is_sneaking");
        write("You give up your silent movements as you shout.\n");
    }

    this_player()->setenv("last_shout_time", sprintf("%d", time()));

    write("You shout, \"" + str + "\"\n");
    this_player()->tell_room_living(environment(this_player()),
        this_player(), 0, " shouts, \"" + str + "\"\n");

    return 1;
}

void help() {
    write(
        "Syntax: shout <message>\n\n"
        "Shouts a message to everyone in your room. No language needed --\n"
        "everyone understands. There is a 5-second cooldown between shouts.\n\n"
        "See also: say, ooc\n"
    );
}
