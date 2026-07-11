/* /cmds/mortal/_suicide.c
   Permanently delete a character level 3 or below.
   Requires typed "YES" confirmation. Blocked in combat and for wizards. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_suicide(string str) {
    object tp;

    tp = this_player();
    if(!tp) return 0;
    if(creatorp(tp)) {
        write("Wizards cannot use suicide.\n");
        return 1;
    }
    if((int)tp->query_level() > 3) {
        write("Suicide is only available for characters level 3 or below.\n");
        write("Characters above level 3 are permanently lost when killed in the Rifts.\n");
        return 1;
    }
    if(sizeof((object *)tp->query_attackers()) > 0) {
        write("You cannot suicide while in combat.\n");
        return 1;
    }
    write(
        "WARNING: This will permanently delete your character.\n"
        "There is no recovery. Your name will be freed for reuse.\n"
        "\nType YES to confirm (anything else cancels): ");
    input_to("confirm_suicide");
    return 1;
}

void confirm_suicide(string str) {
    object tp;
    string name;
    string savefile;

    tp = this_player();
    if(!tp) return;
    if(!str || str != "YES") {
        tell_object(tp, "Suicide cancelled.\n");
        return;
    }
    name     = (string)tp->query_name();
    savefile = sprintf("/secure/save/users/%s/%s.o", name[0..0], name);
    tell_object(tp, "Your character has been permanently deleted. Goodbye.\n");
    tp->remove();
    if(file_size(savefile) >= 0)
        unguarded((: rm, savefile :));
}
