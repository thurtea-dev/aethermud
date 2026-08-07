// /cmds/mortal/_touch.c
// Activate a tattoo at a body location.
// Syntax: touch <location> tattoo [target]
// Examples:
//   touch right forearm tattoo
//   touch right hand tattoo goblin

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_touch(string str) {
    string *words, loc, target_name;
    string resolved, arg_under, tname;
    object target, player;
    mapping tattoos;
    string *locs;
    int tattoo_pos, i, num, j;

    player = this_player();
    if(!player) return 0;

    if(!str || !sizeof(str)) {
        write("Touch what?  Syntax: touch <location> tattoo [target]\n"
              "Example: touch right forearm tattoo\n"
              "         touch right hand tattoo goblin\n");
        return 1;
    }

    words = explode(lower_case(str), " ");

    // Find the word "tattoo" in the input
    tattoo_pos = -1;
    for(i = 0; i < sizeof(words); i++) {
        if(words[i] == "tattoo") { tattoo_pos = i; break; }
    }

    if(tattoo_pos < 0) {
        /* Try matching arg as tattoo name or location */
        resolved = "";
        tattoos  = (mapping)TATTOO_D->query_tattoos(player);
        if(!mapp(tattoos) || !sizeof(keys(tattoos))) return 0;
        locs = keys(tattoos);
        loc  = lower_case(str);

        /* Exact location match */
        if(tattoos[loc]) resolved = loc;

        /* Name match */
        if(!resolved || resolved == "") {
            for(j = 0; j < sizeof(locs); j++) {
                if(lower_case((string)tattoos[locs[j]]) == loc) {
                    resolved = locs[j];
                    break;
                }
            }
        }

        /* Partial / underscore name match */
        if(!resolved || resolved == "") {
            arg_under = replace_string(loc, " ", "_");
            for(j = 0; j < sizeof(locs); j++) {
                tname = replace_string(lower_case((string)tattoos[locs[j]]), " ", "_");
                if(strsrch(tname, arg_under) == 0 || strsrch(arg_under, tname) == 0) {
                    resolved = locs[j];
                    break;
                }
            }
        }

        if(!resolved || resolved == "") return 0;

        TATTOO_D->activate_tattoo(player, resolved, 0);
        return 1;
    }

    // Location is everything before "tattoo"
    if(tattoo_pos == 0) {
        write("Touch which location?  Syntax: touch <location> tattoo [target]\n");
        return 1;
    }
    loc = implode(words[0..(tattoo_pos - 1)], " ");

    // Optional target is everything after "tattoo"
    target_name = 0;
    if(tattoo_pos + 1 < sizeof(words)) {
        target_name = implode(words[(tattoo_pos + 1)..], " ");
    }

    // Resolve optional target
    target = 0;
    if(target_name) {
        target = present(target_name, environment(player));
        if(!target || !living(target)) {
            write("You don't see '" + target_name + "' here.\n");
            return 1;
        }
    }

    // Delegate to TATTOO_D
    TATTOO_D->activate_tattoo(player, loc, target);
    return 1;
}

void init() {
    add_action("cmd_touch", "touch");
}
