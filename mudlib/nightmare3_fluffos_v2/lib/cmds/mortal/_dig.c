/* /cmds/mortal/_dig.c
   Dig command. Requires a wielded shovel.
   Valid dig locations: Splynn Market plaza, Splynn Forest Clearing.
   Both collapse into the same buried chamber. */

#include <std.h>

inherit DAEMON;

#define SPLYNN_MARKET  "/domains/Praxis/areas/splynn/splynn_market"
#define SPLYNN_FOREST  "/domains/Praxis/areas/splynn/splynn_forest_edge"
#define SPLYNN_UNDER   "/domains/Praxis/areas/splynn/splynn_underground"

int cmd_dig(string str) {
    object env, shovel, dest;
    object *inv;
    string base;
    int i;

    env  = environment(this_player());
    base = base_name(env);
    if(base != SPLYNN_MARKET && base != SPLYNN_FOREST) {
        write("There is nothing here worth digging for.\n");
        return 1;
    }
    shovel = 0;
    inv    = all_inventory(this_player());
    i      = sizeof(inv);
    while(i--) {
        if((int)inv[i]->query_property("is_shovel") &&
           objectp((object)inv[i]->query_wielded())) {
            shovel = inv[i];
            break;
        }
    }
    if(!shovel) {
        write("You need a wielded shovel to dig here.\n");
        return 1;
    }
    if(env->query_exit("down")) {
        write("The hole is already open.\n");
        return 1;
    }
    write("You drive the shovel into the soft ground and heave.\n");
    write("The surface cracks, giving way with a grinding roar. Before\n");
    write("you can react, the ground collapses beneath you and you fall\n");
    write("into darkness!\n\n");
    tell_room(env,
        this_player()->query_cap_name() +
        " digs frantically. The ground gives way and they fall into darkness!\n",
        ({ this_player() }));
    dest = load_object(SPLYNN_UNDER);
    env->add_exit("down", SPLYNN_UNDER);
    this_player()->move_player(SPLYNN_UNDER, "down");
    call_out("close_dig", 300, env);
    return 1;
}

void close_dig(object env) {
    if(!env || !objectp(env)) return;
    env->remove_exit("down");
    tell_room(env,
        "The hole in the ground collapses closed with a deep rumble.\n",
        ({}));
}

void help() {
    message("help",
        "Syntax: dig\n\n"
        "Dig into the ground.  Requires a shovel to be wielded.\n"
        "In certain locations this may reveal hidden passages.\n\n"
        "See also: wield",
        this_player());
}
