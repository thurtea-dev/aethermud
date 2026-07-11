#include <std.h>

inherit DAEMON;

int cmd_about(string str) {
    if(!creatorp(this_player())) return 0;
    return (int)"/cmds/creator/_qcs"->qcs_dispatch("about", str);
}

void help() {
    write("Syntax: about <filename|here>\n"
          "Prints raw LPC source for a realm file.\n");
}
