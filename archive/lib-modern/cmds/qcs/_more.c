#include <std.h>

inherit DAEMON;

int cmd_more(string str) {
    if(!creatorp(this_player())) return 0;
    return (int)"/cmds/creator/_qcs"->qcs_dispatch("more", str);
}

void help() {
    write("Syntax: more <filename|here>\n"
          "Paginated view of raw LPC source for a realm file.\n");
}
