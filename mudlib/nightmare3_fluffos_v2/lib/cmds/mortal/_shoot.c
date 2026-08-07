/* /cmds/mortal/_shoot.c
   Thin alias: shoot <target> initiates combat if a ranged weapon is wielded.
   Ammo consumption happens per-round in rifts_combat.c, not here.
   Syntax: shoot <target> */

#include <std.h>

inherit DAEMON;

int cmd_shoot(string str) {
    object victim, weapon;
    object *inv;
    int i;

    if(!str || !strlen(str)) {
        write("Shoot at what?\n");
        return 1;
    }
    if(this_player()->query_ghost()) {
        notify_fail("The dead cannot fight.\n");
        return 0;
    }
    if(environment(this_player())->query_property("no attack")) {
        notify_fail("Greater powers prevent violence here.\n");
        return 0;
    }

    /* Confirm a ranged weapon is wielded before proceeding. */
    weapon = 0;
    inv    = (object *)this_player()->query_wielded();
    if(inv && sizeof(inv)) {
        for(i = 0; i < sizeof(inv); i++) {
            if(inv[i] && (int)inv[i]->query_property("ranged")) {
                weapon = inv[i];
                break;
            }
        }
    }
    if(!weapon) {
        write("You need to wield a ranged weapon first.\n");
        return 1;
    }

    victim = present(lower_case(str), environment(this_player()));
    if(!victim) {
        write("You don't see that here.\n");
        return 1;
    }
    if(victim == this_player()) {
        write("Shoot yourself?\n");
        return 1;
    }
    if(!living(victim)) {
        write("That is not a living target.\n");
        return 1;
    }

    /* Delegate to the standard attack mechanism. */
    return (int)this_player()->kill_ob(victim);
}

void help() {
    message("help",
        "Syntax: shoot <target>\n\n"
        "Open fire on a target using your wielded ranged weapon.\n"
        "Ammo is consumed each round automatically.\n"
        "When the clip is empty your weapon clicks and does no damage.\n"
        "Use reload <weapon> to load a fresh e-clip.\n\n"
        "See also: reload, unload, kill, wield",
        this_player());
}
