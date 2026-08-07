/* /cmds/mortal/_echo.c
   Room echo for roleplay. Message goes to everyone in the room.
   Logged to /log/echo for staff review. */

#include <std.h>

inherit DAEMON;

int cmd_echo(string str) {
    object env;
    string entry;

    if(!str || !sizeof(str)) {
        notify_fail("Echo what?\n");
        return 0;
    }
    if(creatorp(this_player())) {
        write("Wizards use the creator echo command instead.\n");
        return 1;
    }
    env = environment(this_player());
    if(!env) {
        write("You are nowhere.\n");
        return 1;
    }
    message("environment", str, env);
    entry = sprintf("[%s] %s (echo at %s): %s\n",
        ctime(time()), (string)this_player()->query_cap_name(),
        base_name(env), str);
    log_file("echo", entry);
    return 1;
}

void help() {
    write(
        "Syntax: echo <message>\n\n"
        "Sends a message to everyone in your current room without\n"
        "prefixing your name. Use for ambient sound, room events, or\n"
        "narration during roleplay. Abuse is logged.\n\n"
        "See also: say, emote, ooc\n"
    );
}
