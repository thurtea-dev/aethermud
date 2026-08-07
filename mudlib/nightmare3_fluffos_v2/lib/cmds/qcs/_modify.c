#include <std.h>

inherit DAEMON;

int cmd_modify(string str) {
    if(!creatorp(this_player())) return 0;
    if(!str || !sizeof(str)) {
        write("Syntax: modify here <property> <value>\n"
              "        modify <filename> <property> <value>\n");
        return 1;
    }
    if(lower_case(str[0..3]) == "here")
        return (int)"/cmds/creator/_qcs"->qcs_dispatch("modify", str);
    return (int)"/cmds/creator/_qcs"->qcs_dispatch("modify", str);
}

void help() {
    write("Syntax: modify here <property> <value>\n"
          "        modify <filename> <property> <value>\n");
}
