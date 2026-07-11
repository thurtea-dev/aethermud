// /cmds/mortal/_chat.c
// Global in-character chat channel (distinct from OOC).
// chat <message>   -- broadcast to all listening players
// chat off / on    -- toggle receiving chat

#include <std.h>

inherit DAEMON;

int cmd_chat(string str) {
    object *all_players;
    string msg;
    int i;

    if(!creatorp(this_player())) {
        write("That command is for wizard staff only.\n");
        return 1;
    }
    if(!str || str == "") {
        notify_fail("Usage: chat <message>   (or: chat off / chat on)\n");
        return 0;
    }
    if(lower_case(str) == "off") {
        this_player()->setenv("chat_deaf", "1");
        write("Chat channel disabled. Type 'chat on' to re-enable.\n");
        return 1;
    }
    if(lower_case(str) == "on") {
        this_player()->remove_env("chat_deaf");
        write("Chat channel enabled.\n");
        return 1;
    }
    all_players = users();
    for(i = 0; i < sizeof(all_players); i++) {
        if(!all_players[i]) continue;
        if((string)all_players[i]->getenv("chat_deaf") == "1") continue;
        msg = sprintf("[Chat] %s: %s\n",
            (string)this_player()->query_display_name(all_players[i]), str);
        message("chat", msg, all_players[i]);
    }
    return 1;
}

void help() {
    write(
        "Syntax: chat <message>\n"
        "        chat off    stop receiving chat messages\n"
        "        chat on     resume receiving chat messages\n\n"
        "Sends a message on the global in-character chat channel.\n"
        "Wizard staff only. Use OOC for out-of-character discussion.\n\n"
        "See also: ooc, say, radio, shout\n"
    );
}
