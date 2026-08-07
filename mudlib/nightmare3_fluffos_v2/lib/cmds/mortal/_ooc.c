// /cmds/mortal/_ooc.c
// OOC: out-of-character chat, room-local only (like say). Skips
// language checks -- understood by everyone in the room regardless of
// active language. Not a global channel; no toggle, since there is
// nothing to opt in or out of beyond being in the room.

#include <std.h>

inherit DAEMON;

int cmd_ooc(string str) {
    if(!str || str == "") {
        notify_fail("Usage: ooc <message>\n");
        return 0;
    }

    write("You %^RED%^[OOC]%^RESET%^: %^WHITE%^" + str + "%^RESET%^\n");
    this_player()->tell_room_living(environment(this_player()),
        this_player(), 0,
        " %^RED%^[OOC]%^RESET%^: %^WHITE%^" + str + "%^RESET%^\n");
    return 1;
}

void help() {
    write(
        "Syntax: ooc <message>\n\n"
        "Speaks out-of-character to everyone in your current room, the\n"
        "same reach as say. Unlike say, ooc skips the language system --\n"
        "everyone in the room understands it -- and is tagged [OOC] so it\n"
        "reads as out-of-character rather than in-fiction speech.\n\n"
        "There is no global OOC channel; ooc is heard only in your room.\n\n"
        "See also: say, tell, shout\n"
    );
}
