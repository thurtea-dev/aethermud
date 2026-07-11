/* /cmds/mortal/_brief.c
   Brief mode: shorter room descriptions.
   Syntax: brief          -- toggle
           brief on       -- enable
           brief off      -- disable
           brief status   -- show current state */

#include <std.h>

inherit DAEMON;

int cmd_brief(string str) {
    object player;
    int is_brief;

    player = this_player();
    if(!player) return 0;

    /* query_verbose() returns 1 for verbose, 0 for brief */
    is_brief = !player->query_verbose();

    if(!str || !sizeof(str)) {
        player->set_brief();
        is_brief = !player->query_verbose();
        player->setenv("brief_mode", is_brief ? "1" : "0");
        write(is_brief ? "Brief mode on.\n" : "Verbose mode on.\n");
        return 1;
    }

    switch(lower_case(str)) {
    case "on":
        if(is_brief) { write("Brief mode is already on.\n"); return 1; }
        player->set_brief();
        player->setenv("brief_mode", "1");
        write("Brief mode on.\n");
        return 1;

    case "off":
        if(!is_brief) { write("Brief mode is already off.\n"); return 1; }
        player->set_brief();
        player->setenv("brief_mode", "0");
        write("Verbose mode on.\n");
        return 1;

    case "status":
        write("Brief mode is " + (is_brief ? "on" : "off") + ".\n");
        return 1;

    default:
        write("Syntax: brief [on | off | status]\n");
        return 1;
    }
}

void init() {
    add_action("cmd_brief", "brief");
}
