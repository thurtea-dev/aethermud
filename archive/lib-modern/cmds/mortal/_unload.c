/* /cmds/mortal/_unload.c
   Remove remaining ammo from a ranged weapon, return it as a partial e-clip.
   Syntax: unload <weapon> */

#include <std.h>

inherit DAEMON;

int cmd_unload(string str) {
    object weapon, clip;
    object *inv;
    int i, ammo;

    weapon = 0;
    clip   = 0;

    if(!str || !strlen(str)) {
        write("Syntax: unload <weapon>\n");
        return 1;
    }
    inv = all_inventory(this_player());
    for(i = 0; i < sizeof(inv); i++) {
        if(strsrch(lower_case((string)inv[i]->query_short()),
                   lower_case(str)) != -1 &&
           (int)inv[i]->query_property("ranged")) {
            weapon = inv[i];
            break;
        }
    }
    if(!weapon) {
        write("You are not carrying a ranged weapon matching '" + str + "'.\n");
        return 1;
    }
    ammo = (int)weapon->query_property("current_ammo");
    if(ammo <= 0) {
        write((string)weapon->query_short() + " is already empty.\n");
        return 1;
    }

    /* Return remaining charge as a partial e-clip. */
    clip = clone_object("/domains/Praxis/equipment/e_clip.c");
    if(clip) {
        clip->set_property("ammo_count", ammo);
        clip->set_short("a partial e-clip (" + ammo + " shots)");
        clip->move(this_player());
    }
    weapon->set_property("current_ammo", 0);

    write("You eject " + ammo + " shots worth of charge from " +
        (string)weapon->query_short() + " and pocket it.\n");
    say(this_player()->query_cap_name() + " ejects the clip from " +
        (string)weapon->query_short() + ".");
    return 1;
}

void help() {
    message("help",
        "Syntax: unload <weapon>\n\n"
        "Remove remaining ammo from a ranged weapon.\n"
        "Remaining shots are returned as a partial e-clip.\n\n"
        "See also: reload, shoot, inventory",
        this_player());
}
