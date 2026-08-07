#include <std.h>

inherit DAEMON;

int cmd_create(string str) {
    if(!creatorp(this_player())) return 0;
    return (int)"/cmds/creator/_qcs"->qcs_dispatch("create", str);
}

void help() {
    write("Syntax: create room <dir> <file> | create npc|weapon|armor <file>\n");
}
