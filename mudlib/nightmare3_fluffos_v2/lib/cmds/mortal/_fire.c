/* /cmds/mortal/_fire.c
   Thin alias for shoot.  Accepts:
     fire <target>
     fire <weapon> at <target>
   Combat and ammo are handled by rifts_combat.c each round. */

#include <std.h>

inherit DAEMON;

int cmd_fire(string str) {
    string wname, tname;
    object victim, weapon;
    object *inv;
    int i;

    wname = "";
    tname = "";
    weapon = 0;

    if(!str || !strlen(str)) {
        write("Fire at what?\n");
        return 1;
    }
    /* "fire <weapon> at <target>" -- extract just the target. */
    if(sscanf(str, "%s at %s", wname, tname) == 2) {
        str = tname;
    }

    if(this_player()->query_ghost()) {
        notify_fail("The dead cannot fight.\n");
        return 0;
    }
    if(environment(this_player())->query_property("no attack")) {
        notify_fail("Greater powers prevent violence here.\n");
        return 0;
    }

    inv = (object *)this_player()->query_wielded();
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
        write("Fire at yourself?\n");
        return 1;
    }
    if(!living(victim)) {
        write("That is not a living target.\n");
        return 1;
    }

    return (int)this_player()->kill_ob(victim);
}

void help() {
    message("help",
        "Syntax: fire <target>\n"
        "        fire <weapon> at <target>\n\n"
        "Alias for shoot. Opens fire using your wielded ranged weapon.\n"
        "See also: shoot, reload, unload, kill",
        this_player());
}
