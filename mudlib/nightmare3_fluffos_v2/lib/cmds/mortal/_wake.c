// /cmds/mortal/_wake.c
// Wake up from resting.

#include <std.h>

inherit DAEMON;

int cmd_wake(string str) {
    object env;

    if((string)this_player()->getenv("resting") != "1") {
        write("You are not resting.\n");
        return 1;
    }
    this_player()->setenv("resting", "0");
    write("You stand up.\n");
    env = environment(this_player());
    if(env) {
        this_player()->tell_room_living(env, this_player(), 0,
            " stands up.\n");
    }
    return 1;
}

void help() {
    write(
        "Syntax: wake\n\n"
        "Stand up from resting.\n\n"
        "See also: rest, score\n"
    );
}
