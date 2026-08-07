/* /cmds/mortal/_drive.c
   Drive a Tier 1 vehicle (rifts_vehicle.c inheritor) in a direction.
   Player must be inside a vehicle object (is_vehicle=1).
   Both source and destination rooms must have vehicle_accessible=1. */

#include <std.h>
#include <vehicle.h>

inherit DAEMON;

int cmd_drive(string str) {
    object vehicle, room, dest_ob;
    string dest;
    int fuel;

    if(!str || !strlen(str)) {
        write("Drive which direction?\n");
        return 1;
    }
    vehicle = environment(this_player());
    if(!vehicle || !objectp(vehicle) || living(vehicle) ||
       !(int)vehicle->query_property(PROP_IS_VEHICLE)) {
        write("You are not inside a vehicle.\n");
        return 1;
    }
    room = environment(vehicle);
    if(!room) {
        write("The vehicle is not in a valid location.\n");
        return 1;
    }
    if(!(int)room->query_property(PROP_VEHICLE_ACCESSIBLE)) {
        write("You cannot drive a vehicle from here.\n");
        return 1;
    }
    fuel = (int)vehicle->query_fuel();
    if(fuel < 1) {
        write("The vehicle is out of fuel.\n");
        return 1;
    }
    dest = (string)room->query_exit(str);
    if(!dest || !strlen(dest)) {
        write("There is no exit in that direction.\n");
        return 1;
    }
    dest_ob = find_object(dest);
    if(!dest_ob) catch(dest_ob = load_object(dest));
    if(!dest_ob || !(int)dest_ob->query_property(PROP_VEHICLE_ACCESSIBLE)) {
        write("You cannot drive a vehicle there.\n");
        return 1;
    }
    vehicle->vehicle_move(dest);
    write((string)vehicle->query_short() + " slides " + str + ".\n");
    return 1;
}

void help() {
    message("help",
        "Syntax: drive <direction>\n\n"
        "Drive a vehicle you are currently riding in the given direction.\n"
        "Both the current room and the destination must be vehicle-accessible.\n"
        "The vehicle consumes fuel with each move.\n\n"
        "See also: enter, exit",
        this_player());
}
