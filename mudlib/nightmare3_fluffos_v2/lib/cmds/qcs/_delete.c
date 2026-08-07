#include <std.h>

inherit DAEMON;

int cmd_delete(string str) {
    if(!creatorp(this_player())) return 0;
    return (int)"/cmds/creator/_qcs"->qcs_dispatch("delete", str);
}

void help() {
    write("Syntax: delete <object>\n"
          "Removes an object from the current room (QCS, realm only).\n");
}
