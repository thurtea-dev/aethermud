/* /cmds/mortal/_prowl.c
   Syntax: prowl
   Toggles prowl mode for characters with the Prowl skill.
   While prowling, each move rolls Prowl in move_player (std/user.c):
   success hides the departure and arrival messages for that move.
   Prowl mode ends when the player attacks, speaks, shouts, or types
   prowl again. Shares the is_sneaking property with sneak/hide and
   the assassination ambush check. */

#include <std.h>

inherit DAEMON;

int cmd_prowl(string str) {
    object player;
    int skill;

    player = this_player();
    if((int)player->query_ghost()) {
        write("Ghosts drift silently already.\n");
        return 1;
    }
    skill = (int)player->query_skill("prowl");
    if(skill <= 0) {
        write("You do not have the Prowl skill.\n");
        return 1;
    }
    if((int)player->query_property("is_sneaking")) {
        player->remove_property("is_sneaking");
        write("You relax and stop prowling.\n");
        return 1;
    }
    player->set_property("is_sneaking", 1);
    write("You begin prowling, moving carefully and quietly.\n");
    return 1;
}

void help() {
    write(
        "Syntax: prowl\n\n"
        "Toggles prowl mode. Requires the Prowl skill.\n"
        "While prowling, every move rolls your Prowl skill. On a success,\n"
        "others do not see you leave or arrive. On a failure, you are seen\n"
        "moving normally, but prowl mode stays active.\n"
        "Prowl mode ends when you attack, speak, shout, or type prowl again.\n\n"
        "See also: sneak, hide, assassination\n"
    );
}
