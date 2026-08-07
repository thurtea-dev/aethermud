// /cmds/mortal/_start.c
// Set your login location to the current room.

#include <std.h>

inherit DAEMON;

int cmd_start(string str) {
    string step;

    step = (string)this_player()->getenv("creation_step");
    if(step && step != "" && step != "done") return 0;
    if(!str || str != "here") {
        notify_fail("Syntax: start here\n");
        return 0;
    }
    return (int)this_player()->set_start_here();
}

void help() {
    write("Syntax: start here\n\n"
        "Sets your current room as where you login next time.\n"
        "Your logout location is saved automatically when you quit.\n");
}
