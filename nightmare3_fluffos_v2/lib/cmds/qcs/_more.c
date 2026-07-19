#include <std.h>

inherit DAEMON;

int cmd_more(string str) {
    object tp;

    tp = this_player();
    if(!creatorp(tp)) return 0;
    /* No QCS eligibility or no staff of creation: decline silently so
       the command search falls through to the generic more pager
       instead of printing a QCS gate error over an unrelated command. */
    if(!admin_wizp(tp) && !coding_wizp(tp)) return 0;
    if(!(int)has_wiz_tool(tp, "staff_of_creation")) return 0;
    return (int)"/cmds/creator/_qcs"->qcs_dispatch("more", str);
}

void help() {
    write("Syntax: more <filename|here>\n"
          "Paginated view of raw LPC source for a realm file.\n");
}
