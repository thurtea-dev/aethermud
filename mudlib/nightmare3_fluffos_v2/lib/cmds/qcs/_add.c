#include <std.h>

inherit DAEMON;

int cmd_add(string str) {
    if(!creatorp(this_player())) return 0;
    return (int)"/cmds/creator/_qcs"->qcs_dispatch("add", str);
}

void help() {
    write("Syntax: add <npcfile> to here\n"
          "        add <npcfile> to <roomfile>\n");
}
