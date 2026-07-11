// /cmds/mortal/_stop.c
// Stop combat command.  Ends your attacks against current targets.
// If the target is non-aggressive, combat ends both ways.
// If the target is aggressive, it will keep attacking you.
// Syntax: stop

#include <std.h>

inherit DAEMON;

int cmd_stop(string str) {
    object player, *atk;
    int i, aggressive_count;

    player = this_player();
    if(!player) return 0;

    atk = (object *)player->query_attackers();
    if(!sizeof(atk)) {
        write("You are not in combat.\n");
        return 1;
    }

    aggressive_count = 0;

    // Tell any non-aggressive enemies to stop fighting back.
    for(i = 0; i < sizeof(atk); i++) {
        if(!objectp(atk[i])) continue;
        if((int)atk[i]->query_property("aggressive")) {
            aggressive_count++;
        } else {
            catch(atk[i]->cease_all_attacks());
        }
    }

    catch(player->cease_all_attacks());
    write("You stop fighting.\n");

    if(aggressive_count > 0) {
        for(i = 0; i < sizeof(atk); i++) {
            if(!objectp(atk[i])) continue;
            if((int)atk[i]->query_property("aggressive"))
                write((string)atk[i]->query_cap_name() +
                      " is still attacking you!\n");
        }
    }
    return 1;
}

void init() {
    add_action("cmd_stop", "stop");
}
