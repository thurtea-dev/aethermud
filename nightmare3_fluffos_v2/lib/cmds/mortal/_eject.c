/* /cmds/mortal/_eject.c
   Eject from a vehicle or remove power armor quickly.
   Syntax: eject

   Tier 1 (simple vehicle, is_vehicle=1 on container):
     Player is inside the vehicle object.
     Move player to environment(vehicle) to eject.

   Tier 2 (multi-room vehicle, vehicle_interior=1 on room):
     Player is inside one of the vehicle's interior rooms.
     Each interior room stores the controlling daemon path as vehicle_daemon.
     Daemon provides query_current_room() for the outside position. */

#include <std.h>
#include <vehicle.h>

inherit DAEMON;

int cmd_eject(string str) {
    object env, dest, vd;
    object *worn;
    object armor;
    string dest_path, vd_path;
    int i;

    env = environment(this_player());

    /* Tier 1: player is inside a vehicle container (is_vehicle=1). */
    if(env && (int)env->query_property(PROP_IS_VEHICLE)) {
        dest = environment(env);
        if(!dest) {
            write("There is nowhere to eject to.\n");
            return 1;
        }
        this_player()->move(dest);
        write("You eject from the vehicle.\n");
        say(this_player()->query_cap_name() + " ejects from the vehicle.");
        return 1;
    }

    /* Tier 2: player is inside a multi-room vehicle interior (vehicle_interior=1). */
    /* Daemon path is stored on the room as vehicle_daemon. */
    if(env && (int)env->query_property(PROP_VEHICLE_INTERIOR)) {
        vd_path = (string)env->query_property(PROP_VEHICLE_DAEMON);
        if(!vd_path || !strlen(vd_path)) {
            write("Cannot locate vehicle system. Contact an admin.\n");
            return 1;
        }
        vd = find_object(vd_path);
        if(!vd) catch(vd = load_object(vd_path));
        dest_path = vd ? (string)vd->query_current_room() : "";
        if(!dest_path || !strlen(dest_path)) {
            write("Cannot determine vehicle location. Contact an admin.\n");
            return 1;
        }
        dest = find_object(dest_path);
        if(!dest) catch(dest = load_object(dest_path));
        if(!dest) {
            write("Cannot eject: vehicle location not accessible.\n");
            return 1;
        }
        this_player()->move(dest);
        write("You pull the emergency release and tumble free of the vehicle.\n");
        say(this_player()->query_cap_name() + " ejects from the vehicle.");
        return 1;
    }

    /* Power armor: check worn items for power_armor property. */
    worn  = (object *)this_player()->query_worn();
    if(!worn) worn = ({});
    armor = 0;
    i     = sizeof(worn);
    while(i--) {
        if(worn[i] && (int)worn[i]->query_property("power_armor")) {
            armor = worn[i];
            break;
        }
    }
    if(armor) {
        armor->unequip();
        write("You eject from " + (string)armor->query_short() + ".\n");
        say(this_player()->query_cap_name() + " ejects from " +
            (string)armor->query_short() + ".");
        return 1;
    }

    write("You are not inside a vehicle or power armor.\n");
    return 1;
}

void help() {
    message("help",
        "Syntax: eject\n\n"
        "Quickly eject from a vehicle or remove power armor.\n"
        "Works from inside any vehicle (simple or multi-room)\n"
        "or while wearing power armor.\n\n"
        "See also: wear, remove, enter, score",
        this_player());
}
