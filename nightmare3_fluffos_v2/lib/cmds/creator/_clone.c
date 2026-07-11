#include <std.h>

inherit DAEMON;

int cmd_clone(string str) {
    if(!creatorp(this_player())) return 0;
    return (int)"/cmds/creator/_qcs"->qcs_dispatch("clone", str);
}

void help() {
    write("Syntax: clone <filename>\n"
          "Clones a realm file into your inventory.\n");
}
