// /cmds/mortal/_exp.c  --  alias for 'experience'
#include <std.h>
inherit DAEMON;
int cmd_exp(string str) {
    return (int)call_other("/cmds/mortal/_experience", "cmd_experience", str);
}
void help() { call_other("/cmds/mortal/_experience", "help"); }
