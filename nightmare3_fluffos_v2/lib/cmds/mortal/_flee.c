// /cmds/mortal/_flee.c
// Manual flee command.  Costs 1 APM.  PP-based success chance (min 30%).
// Syntax: flee

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_flee(string str) {
    object player, env;
    object *exits_obs, *attackers;
    string *exits, dir;
    int pp, chance, roll;

    player = this_player();
    if(!player) return 0;
    env = environment(player);

    attackers = (object *)player->query_attackers();
    if(!sizeof(attackers)) {
        write("You are not in combat.\n");
        return 1;
    }

    // Costs 1 APM
    if(!(int)RIFTS_COMBAT_D->can_do_attack(player)) {
        write("You have no attacks left this melee round to flee!\n");
        return 1;
    }
    RIFTS_COMBAT_D->use_rifts_attack(player);

    // PP-based chance: (PP / 30) * 100%, floor at 30%
    pp     = (int)player->query_stat("PP");
    if(pp <= 0) pp = 10;
    chance = (pp * 100) / 30;
    if(chance < 30) chance = 30;
    if(chance > 95) chance = 95;

    roll = random(100) + 1;

    if(roll > chance) {
        // Failed
        player->tell_room_living(env, player, 0,
            " tries to flee but fails!\n");
        write("You stumble trying to escape!  (rolled " + roll +
              " vs " + chance + "% needed)\n");
        return 1;
    }

    // Success - find a random exit
    exits = (string *)(env ? env->query_exits() : ({}));
    if(!exits || !sizeof(exits)) exits = ({});
    if(!sizeof(exits)) {
        write("There is nowhere to flee to!\n");
        return 1;
    }

    dir = exits[random(sizeof(exits))];

    write("You turn and flee " + dir + "!\n");
    player->tell_room_living(env, player, 0, " turns and flees!\n");

    // End combat
    {
        object *atk;
        int i;
        atk = (object *)player->query_attackers();
        for(i = 0; i < sizeof(atk); i++) {
            if(objectp(atk[i])) {
                catch(atk[i]->stop_attack(player));
            }
        }
        catch(player->cease_all_attacks());
    }

    // Move
    player->force_me(dir);
    return 1;
}

void init() {
    add_action("cmd_flee", "flee");
}
