/* /cmds/creator/_qcsdirs.c
 * Create missing QCS realm directories for a wizard realm.
 */

#include <std.h>

inherit DAEMON;

int cmd_qcsdirs(string str) {
    string target;
    string role;

    if(!creatorp(this_player())) return 0;
    if(str && sizeof(str)) {
        role = (string)this_player()->getenv("wiz_role");
        if(!archp(this_player()) && role != "admin") {
            write("Only admins can bootstrap another wizard's realm.\n");
            return 1;
        }
        target = lower_case(str);
    } else
        target = lower_case((string)this_player()->query_name());
    if(!(int)"/cmds/creator/_qcs"->qcs_ensure_realm_dirs(target)) {
        write("QCS: failed to create directories for " + target + ".\n");
        return 1;
    }
    write("QCS: realm directories ready under /realms/" + target + "/area/\n");
    write("  room/ npc/ weap/ armor/\n");
    return 1;
}

void help() {
    write(
        "Syntax: qcsdirs\n"
        "        qcsdirs <wizard>\n\n"
        "Creates missing QCS directories under a wizard realm:\n"
        "  /realms/<wiz>/area/room/\n"
        "  /realms/<wiz>/area/npc/\n"
        "  /realms/<wiz>/area/weap/\n"
        "  /realms/<wiz>/area/armor/\n\n"
        "With no argument, bootstraps your own realm.\n"
        "Admins may pass a wizard name to fix another realm.\n"
        "Safe to run more than once.\n"
    );
}
