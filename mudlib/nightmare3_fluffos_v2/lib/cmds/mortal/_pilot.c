// /cmds/mortal/_pilot.c
// Mount, check status, or dismount a vehicle.
// Requires an appropriate pilot skill for the vehicle type.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_pilot(string str) {
    object vehicle;
    object room;
    object *contents;
    string short_name;
    string vtype;
    string pilot_skill;
    string cur_piloting;
    int skill_pct;
    int roll;
    int speed;
    int mdc;
    int i;
    int level;

    if(!str || !sizeof(str)) {
        cur_piloting = (string)this_player()->query_property("piloting");
        if(cur_piloting && sizeof(cur_piloting)) {
            speed = (int)this_player()->query_property("vehicle_speed");
            mdc   = (int)this_player()->query_property("vehicle_mdc");
            write("Currently piloting: " + cur_piloting + "\n"
                  "  Speed: " + speed + " mph\n"
                  "  Vehicle MDC: " + mdc + "\n");
        } else {
            write("You are not piloting any vehicle.\n"
                  "Syntax: pilot <vehicle> | pilot | pilot off\n");
        }
        return 1;
    }

    if(str == "off") {
        cur_piloting = (string)this_player()->query_property("piloting");
        if(!cur_piloting || !sizeof(cur_piloting)) {
            write("You are not piloting any vehicle.\n");
            return 1;
        }
        short_name = cur_piloting;
        this_player()->set_property("piloting", "");
        this_player()->set_property("vehicle_speed", 0);
        this_player()->set_property("vehicle_mdc", 0);
        this_player()->set_property("in_vehicle", 0);
        write("You dismount the " + short_name + ".\n");
        say(this_player()->query_cap_name() + " dismounts from " + short_name + ".");
        return 1;
    }

    room = environment(this_player());
    if(!room) {
        write("You are not in a valid location.\n");
        return 1;
    }

    vehicle  = 0;
    contents = all_inventory(room);
    for(i = 0; i < sizeof(contents); i++) {
        if(living(contents[i])) continue;
        if(!(int)contents[i]->query_property("is_vehicle")) continue;
        if(strsrch(lower_case((string)contents[i]->query_short()),
                   lower_case(str)) != -1 ||
           strsrch(lower_case((string)contents[i]->query_name()),
                   lower_case(str)) != -1) {
            vehicle = contents[i];
            break;
        }
    }

    if(!vehicle) {
        write("You don't see any vehicle matching '" + str + "' here.\n");
        return 1;
    }

    short_name = (string)vehicle->query_short();
    if(!short_name || !sizeof(short_name)) short_name = "vehicle";

    vtype = (string)vehicle->query_property("vehicle_type");
    if(!vtype || !sizeof(vtype)) {
        if((int)vehicle->query_property("hovercycle") ||
           (int)vehicle->query_property("is_hovercycle"))
            vtype = "hovercycle";
        else if((int)vehicle->query_property("aircraft") ||
                (int)vehicle->query_property("is_aircraft"))
            vtype = "aircraft";
        else
            vtype = "ground";
    }

    pilot_skill = "";
    if(vtype == "hovercycle") {
        if((int)this_player()->query_skill("pilot hovercycle") > 0)
            pilot_skill = "pilot hovercycle";
        else if((int)this_player()->query_skill("pilot hovercraft") > 0)
            pilot_skill = "pilot hovercraft";
        else if((int)this_player()->query_skill("pilot hover vehicle") > 0)
            pilot_skill = "pilot hover vehicle";
        else if((int)this_player()->query_skill("pilot two vehicles of choice") > 0)
            pilot_skill = "pilot two vehicles of choice";
    } else if(vtype == "aircraft") {
        if((int)this_player()->query_skill("pilot aircraft") > 0)
            pilot_skill = "pilot aircraft";
        else if((int)this_player()->query_skill("pilot jet pack") > 0)
            pilot_skill = "pilot jet pack";
        else if((int)this_player()->query_skill("pilot two vehicles of choice") > 0)
            pilot_skill = "pilot two vehicles of choice";
    } else {
        if((int)this_player()->query_skill("pilot ground vehicles") > 0)
            pilot_skill = "pilot ground vehicles";
        else if((int)this_player()->query_skill("pilot automobile") > 0)
            pilot_skill = "pilot automobile";
        else if((int)this_player()->query_skill("pilot hovercraft") > 0)
            pilot_skill = "pilot hovercraft";
        else if((int)this_player()->query_skill("pilot two vehicles of choice") > 0)
            pilot_skill = "pilot two vehicles of choice";
    }

    if(!pilot_skill || !sizeof(pilot_skill)) {
        write("You don't know how to pilot this type of vehicle.\n");
        return 1;
    }

    skill_pct = (int)this_player()->query_skill(pilot_skill);
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    skill_pct += (level - 1) * 5;
    if(skill_pct > 98) skill_pct = 98;
    skill_pct += (int)ADDICTION_D->query_skill_modifier(this_player());
    if(skill_pct < 1) skill_pct = 1;
    roll      = random(100) + 1;
    speed     = (int)vehicle->query_property("speed");
    mdc       = (int)vehicle->query_property("mdc");
    if(speed < 1) speed = 100;
    if(mdc < 1)   mdc   = 50;

    this_player()->set_property("piloting", short_name);
    this_player()->set_property("vehicle_speed", speed);
    this_player()->set_property("vehicle_mdc", mdc);
    this_player()->set_property("in_vehicle", 1);

    write("You climb into/onto the " + short_name +
          " and the systems come online.  Speed: " + speed + " mph.\n");
    if(roll > skill_pct) {
        write("You struggle with the controls but manage to get it running.\n");
    }
    say(this_player()->query_cap_name() + " mounts " + short_name + ".");
    return 1;
}

void help() {
    message("help",
        "Syntax: pilot <vehicle>  -- enter/mount a vehicle\n"
        "        pilot            -- show current vehicle status\n"
        "        pilot off        -- dismount current vehicle\n\n"
        "Pilot allows you to mount and operate vehicles in the world.\n"
        "You must have an appropriate pilot skill:\n"
        "  Ground vehicles: pilot ground vehicles, pilot automobile\n"
        "  Hovercycles:     pilot hovercycle, pilot hovercraft\n"
        "  Aircraft:        pilot aircraft\n\n"
        "While piloting, vehicle MDC absorbs damage in combat instead of\n"
        "your personal MDC/SDC.  When vehicle MDC reaches 0, you are ejected\n"
        "and the vehicle is destroyed.\n\n"
        "See also: drive, score",
        this_player());
}
