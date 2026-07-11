/* /cmds/mortal/_reload.c
   Load an e-clip into a ranged weapon.
   Syntax: reload <weapon>
           reload <weapon> with <clip> */

#include <std.h>

inherit DAEMON;

int cmd_reload(string str) {
    string wname, cname;
    object weapon, clip;
    object *inv;
    int i, new_ammo, clip_ammo, ammo_max;

    wname  = "";
    cname  = "";
    weapon = 0;
    clip   = 0;

    if(!str || !strlen(str)) {
        write("Syntax: reload <weapon>\n"
              "        reload <weapon> with <clip>\n");
        return 1;
    }
    if(sscanf(str, "%s with %s", wname, cname) != 2) {
        wname = str;
        cname = "";
    }
    wname = lower_case(wname);
    cname = lower_case(cname);

    inv = all_inventory(this_player());

    /* Find the ranged weapon in inventory by name. */
    for(i = 0; i < sizeof(inv); i++) {
        if(strsrch(lower_case((string)inv[i]->query_short()), wname) != -1 &&
           (int)inv[i]->query_property("ranged")) {
            weapon = inv[i];
            break;
        }
    }
    if(!weapon) {
        write("You are not carrying a ranged weapon matching '" + wname + "'.\n");
        return 1;
    }

    /* Find the clip. */
    if(sizeof(cname)) {
        for(i = 0; i < sizeof(inv); i++) {
            if(strsrch(lower_case((string)inv[i]->query_short()), cname) != -1 &&
               (int)inv[i]->query_property("e_clip")) {
                clip = inv[i];
                break;
            }
        }
        if(!clip) {
            write("You are not carrying '" + cname + "'.\n");
            return 1;
        }
    } else {
        for(i = 0; i < sizeof(inv); i++) {
            if((int)inv[i]->query_property("e_clip")) {
                clip = inv[i];
                break;
            }
        }
        if(!clip) {
            write("You have no e-clips to load.\n");
            return 1;
        }
    }

    clip_ammo = (int)clip->query_property("ammo_count");
    if(clip_ammo <= 0) {
        write((string)clip->query_short() + " is empty.\n");
        return 1;
    }
    ammo_max  = (int)weapon->query_property("ammo_max");
    if(!ammo_max) ammo_max = 30;
    new_ammo  = (clip_ammo < ammo_max) ? clip_ammo : ammo_max;

    weapon->set_property("current_ammo", new_ammo);
    clip->destruct();

    write("You reload " + (string)weapon->query_short() +
        ". (" + new_ammo + " shots loaded)\n");
    say(this_player()->query_cap_name() + " reloads " +
        (string)weapon->query_short() + ".");
    return 1;
}

void help() {
    message("help",
        "Syntax: reload <weapon>\n"
        "        reload <weapon> with <clip>\n\n"
        "Load an e-clip into a ranged weapon. With no clip specified,\n"
        "the first e-clip found in your inventory is used.\n\n"
        "Example: reload laser pistol\n"
        "         reload rifle with long e-clip\n\n"
        "See also: unload, shoot, fire, inventory",
        this_player());
}
