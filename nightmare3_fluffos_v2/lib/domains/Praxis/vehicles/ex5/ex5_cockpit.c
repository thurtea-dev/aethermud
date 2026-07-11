// /d/Praxis/vehicles/ex5/ex5_cockpit.c
// EX-5 Behemoth -- pilot cockpit.
// Intercepts directional commands when powered; shows HUD overlay on room desc.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

#define EX5_D "/daemon/ex5_d"

static private string *__directions;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_property("vehicle_interior", 1);
    set_property("vehicle_daemon", "/daemon/ex5_d");
    __directions = ({ "north", "south", "east", "west", "up", "down",
                      "northeast", "northwest", "southeast", "southwest",
                      "ne", "nw", "se", "sw" });
    set_short("EX-5 Behemoth: Cockpit");
    set_long(
        "The pilot cockpit of the EX-5 Behemoth. Every surface is screens,\n"
        "controls, and targeting systems. The pilot seat is a heavily padded\n"
        "command chair with integrated restraints. Wraparound display panels\n"
        "show external camera feeds, currently showing the chamber walls.\n\n"
        "Two secondary stations line the rear wall for weapons and navigation.\n"
        "Everything is powered down, screens dark. The silence is complete.\n\n"
        "Type 'power on' to start the EX-5.\n"
        "Type 'power off' to shut it down.\n"
        "Type 'status' for current stats.\n"
        "When powered: enter a direction to pilot the EX-5.\n\n"
        "Exit: down to the main hull.");
    set_exits( ([
        "down" : "/domains/Praxis/vehicles/ex5/ex5_hull"
    ]) );
    set_listen("default",
        "Silence, except for a faint ticking from the cooling displays.");
    set_smell("default",
        "Plastic, metal, and the ghost of long-ago human occupation.");
    set_items( ([
        "screens" : "Wraparound display panels. Currently dark. When powered they show\n"
                     "external camera feeds from all around the EX-5.",
        "seat" : "The pilot command chair. Padded, with integrated restraints and\n"
                     "direct neural interface ports that are mercifully unplugged.",
        "controls" : "Hundreds of switches, sliders, and joystick assemblies. Most are\n"
                     "labeled in pre-Rifts technical script.",
        "targeting": "Targeting system displays at the weapons station. Dark.",
        "stations" : "Two rear stations for weapons and navigation. Functional but\n"
                     "currently dark.",
        "camera" : "External camera feeds showing the chamber walls around the EX-5.\n"
                     "Dim but functional."
    ]) );
}

varargs string query_long(string str) {
    object ex5d;

    if(str) return ::query_long(str);
    ex5d = find_object(EX5_D);
    if(!ex5d) catch(ex5d = load_object(EX5_D));
    if(ex5d && (int)ex5d->query_is_powered() &&
       this_player() && environment(this_player()) == this_object())
        return (string)ex5d->query_hud(this_player()) + ::query_long();
    return ::query_long();
}

void init() {
    ::init();
    add_action("pilot_cmd", "", 1);
}

int pilot_cmd(string str) {
    object ex5d, p;
    string s, pname, target;
    int i;

    if(!str || !strlen(str)) return 0;
    ex5d = find_object(EX5_D);
    if(!ex5d) catch(ex5d = load_object(EX5_D));
    if(!ex5d) return 0;
    s = lower_case(str);
    if(s == "power on") {
        if((int)this_player()->query_skill("pilot robot combat elite") <= 0 &&
           (int)this_player()->query_skill("pilot robot") <= 0 &&
           (int)this_player()->query_skill("pilot two vehicles of choice") <= 0 &&
           (int)this_player()->query_skill("pilot ex-5") <= 0) {
            write("You have no idea how to operate this machine.\n"
                  "Pilot Robot Combat Elite or Pilot EX-5 skill is required.\n");
            return 1;
        }
        ex5d->power_on(this_player());
        return 1;
    }
    if(s == "power off") {
        ex5d->power_off(this_player());
        return 1;
    }
    if(s == "status") {
        p = (object)ex5d->query_pilot();
        pname = (p && objectp(p)) ? (string)p->query_cap_name() : "none";
        write(sprintf(
            "EX-5 Behemoth Status\n"
            "--------------------\n"
            "Power: %s\n"
            "Fuel: %d/%d\n"
            "Location: %s\n"
            "Heading: %s\n"
            "Pilot: %s\n"
            "Crew: %d\n",
            (int)ex5d->query_is_powered() ? "ONLINE" : "OFFLINE",
            (int)ex5d->query_fuel(), (int)ex5d->query_max_fuel(),
            (string)ex5d->query_current_room(),
            (string)ex5d->query_last_dir(),
            pname,
            sizeof((object *)ex5d->query_passengers())
        ));
        return 1;
    }
    if(!(int)ex5d->query_is_powered()) return 0;
    if(s[0..3] == "fire") {
        target = (strlen(s) > 5) ? s[5..] : "";
        ex5d->fire_weapon(this_player(), target);
        return 1;
    }
    i = sizeof(__directions);
    while(i--) {
        if(s == __directions[i]) {
            ex5d->pilot_move(this_player(), s);
            return 1;
        }
    }
    return 0;
}

void reset() {
    ::reset();
}
