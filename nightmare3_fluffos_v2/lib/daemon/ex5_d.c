// /daemon/ex5_d.c
// EX-5 Behemoth control daemon.
// Tracks current position, power state, pilot, and movement of the EX-5.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

#define EX5_HULL     "/domains/Praxis/vehicles/ex5/ex5_hull"
#define EX5_COCKPIT  "/domains/Praxis/vehicles/ex5/ex5_cockpit"
#define EX5_FWDBAY   "/domains/Praxis/vehicles/ex5/ex5_forward_bay"
#define EX5_QUARTERS "/domains/Praxis/vehicles/ex5/ex5_crew_quarters"
#define EX5_PORTSIDE "/domains/Praxis/vehicles/ex5/ex5_portside"
#define EX5_CARGO    "/domains/Praxis/vehicles/ex5/ex5_cargo"
#define EX5_START    "/domains/Praxis/areas/splynn/splynn_underground"

private string  current_room;
private int     is_powered;
private object  pilot;
private int     fuel;
private int     max_fuel;
private string  last_dir;
private int     last_fire_time;
private int     hull_locked;
private int     hull_pin;

#define WEAPON_COOLDOWN 5
#define PARTICLE_BEAM   "EX-5 particle beam"

void create() {
    ::create();
    current_room   = EX5_START;
    is_powered     = 0;
    pilot          = 0;
    fuel           = 50;
    max_fuel       = 50;
    last_dir       = "none";
    last_fire_time = 0;
    hull_locked    = 0;
    hull_pin       = 0;
}

int query_hull_locked() { return hull_locked; }

int try_unlock_hull(int pin) {
    if(!hull_locked) return 1;
    if(pin == hull_pin) {
        hull_locked = 0;
        return 1;
    }
    return 0;
}

void lock_hull(int pin) {
    if(pin < 0 || pin > 9999) return;
    hull_pin = pin;
    hull_locked = 1;
}

string query_current_room() { return current_room; }
int    query_is_powered()   { return is_powered; }
object query_pilot()        { return pilot; }
int    query_fuel()         { return fuel; }
int    query_max_fuel()     { return max_fuel; }
string query_last_dir()     { return last_dir; }

void set_fuel(int v)            { fuel = v; }
void set_current_room(string s) { current_room = s; }

object *query_passengers() {
    object hull, fwd, qtr, cpit, pside, cargo;
    object *pass, *inv;
    int i;

    pass  = ({});
    hull  = find_object(EX5_HULL);
    fwd   = find_object(EX5_FWDBAY);
    qtr   = find_object(EX5_QUARTERS);
    cpit  = find_object(EX5_COCKPIT);
    pside = find_object(EX5_PORTSIDE);
    cargo = find_object(EX5_CARGO);
    if(hull) {
        inv = all_inventory(hull);
        i   = sizeof(inv);
        while(i--) if(living(inv[i])) pass += ({ inv[i] });
    }
    if(fwd) {
        inv = all_inventory(fwd);
        i   = sizeof(inv);
        while(i--) if(living(inv[i])) pass += ({ inv[i] });
    }
    if(qtr) {
        inv = all_inventory(qtr);
        i   = sizeof(inv);
        while(i--) if(living(inv[i])) pass += ({ inv[i] });
    }
    if(cpit) {
        inv = all_inventory(cpit);
        i   = sizeof(inv);
        while(i--) if(living(inv[i])) pass += ({ inv[i] });
    }
    if(pside) {
        inv = all_inventory(pside);
        i   = sizeof(inv);
        while(i--) if(living(inv[i])) pass += ({ inv[i] });
    }
    if(cargo) {
        inv = all_inventory(cargo);
        i   = sizeof(inv);
        while(i--) if(living(inv[i])) pass += ({ inv[i] });
    }
    return pass;
}

string query_hud(object who) {
    object loc_ob;
    string loc_short, pilot_name;
    int crew;

    loc_ob    = find_object(current_room);
    loc_short = loc_ob ? (string)loc_ob->query_short() : "unknown";
    if(!loc_short || !strlen(loc_short)) loc_short = "unknown";
    pilot_name = (pilot && objectp(pilot)) ? (string)pilot->query_cap_name() : "none";
    crew       = sizeof(query_passengers());
    return sprintf(
        "\n=== EX-5 BEHEMOTH COCKPIT HUD ===\n"
        "Location: %-28s Heading: %s\n"
        "Hull MDC: 800/800    Fuel: %d/%d    Speed: 1\n"
        "Pilot: %-20s Crew: %d\n"
        "=================================\n",
        loc_short, last_dir,
        fuel, max_fuel,
        pilot_name, crew
    );
}

void power_on(object who) {
    object cpit;

    if(!who || !objectp(who)) return;
    if(is_powered) {
        write("The EX-5 is already powered.\n");
        return;
    }
    is_powered = 1;
    pilot      = who;
    cpit       = find_object(EX5_COCKPIT);
    if(cpit)
        tell_room(cpit,
            "\n*** EX-5 BEHEMOTH POWER SYSTEMS ONLINE ***\n"
            "Reactor engaged.  All systems nominal.\n"
            "Hull MDC: 800/800.  Fuel: "+fuel+"/"+max_fuel+".\n"
            "Pilot interface active.  Enter directions to move.\n\n",
            ({}));
}

void power_off(object who) {
    object cpit;

    if(!who || !objectp(who)) return;
    if(!is_powered) {
        write("The EX-5 is already offline.\n");
        return;
    }
    is_powered = 0;
    pilot      = 0;
    cpit       = find_object(EX5_COCKPIT);
    if(cpit)
        tell_room(cpit,
            "\n*** EX-5 BEHEMOTH POWER SYSTEMS OFFLINE ***\n"
            "Reactor shutdown.  All displays dark.\n\n",
            ({}));
}

void pilot_move(object who, string dir) {
    object dest_ob, from_ob, hull, fwd, qtr, cpit, pside, cargo;
    object *passengers;
    string dest_path;
    int i;

    if(!is_powered) {
        if(who) write("The EX-5 is not powered.  Type 'power on' to start it.\n");
        return;
    }
    if(fuel < 1) {
        if(who) write("The EX-5 is out of fuel.  Return to the buried chamber to refuel.\n");
        return;
    }
    from_ob = find_object(current_room);
    if(!from_ob) {
        if(who) write("EX-5 location error.  Contact an admin.\n");
        return;
    }
    dest_path = (string)from_ob->query_exit(dir);
    if(!dest_path || !strlen(dest_path)) {
        if(who) write("The EX-5 cannot move that way. No exit in that direction.\n");
        return;
    }
    dest_ob = find_object(dest_path);
    if(!dest_ob) catch(dest_ob = load_object(dest_path));
    if(!dest_ob) {
        if(who) write("The EX-5 cannot move there.\n");
        return;
    }
    if(!(int)dest_ob->query_property("vehicle_accessible") &&
       !(int)dest_ob->query_property("ex5_accessible")) {
        if(who) write("The EX-5 cannot move into that area.\n");
        return;
    }
    tell_room(from_ob,
        "The EX-5 Behemoth turns and thunders away to the "+dir+".\n",
        ({}));
    current_room = dest_path;
    last_dir     = dir;
    fuel--;
    hull  = find_object(EX5_HULL);
    fwd   = find_object(EX5_FWDBAY);
    qtr   = find_object(EX5_QUARTERS);
    cpit  = find_object(EX5_COCKPIT);
    pside = find_object(EX5_PORTSIDE);
    cargo = find_object(EX5_CARGO);
    if(hull)  hull->add_exit("out",  dest_path);
    if(fwd)   fwd->add_exit("out",   dest_path);
    if(qtr)   qtr->add_exit("out",   dest_path);
    if(pside) pside->add_exit("out", dest_path);
    if(cargo) cargo->add_exit("out", dest_path);
    tell_room(dest_ob,
        "The ground shakes as an enormous humanoid machine stomps into the area from the "+
        dir+"!\n",
        ({}));
    passengers = query_passengers();
    i = sizeof(passengers);
    while(i--)
        if(passengers[i] && objectp(passengers[i]))
            catch(passengers[i]->describe_current_room(1));
}

void refuel(object who) {
    if(current_room != EX5_START) {
        if(who) write("The EX-5 must be in the buried chamber to refuel.\n");
        return;
    }
    fuel = max_fuel;
    if(who) write("The EX-5 fuel reserves restored to "+fuel+"/"+max_fuel+".\n");
}

void fire_weapon(object who, string target_name) {
    object loc_ob;
    object target;
    int dmg;
    int now;
    int remaining;
    string tname;

    if(!is_powered) {
        if(who) write("The EX-5 is not powered. Type 'power on' first.\n");
        return;
    }
    if(!target_name || !strlen(target_name)) {
        if(who) write("Fire at what? Syntax: fire <target>\n");
        return;
    }

    now       = time();
    remaining = WEAPON_COOLDOWN - (now - last_fire_time);
    if(last_fire_time > 0 && remaining > 0) {
        if(who)
            write("Particle beam capacitors still recharging. (" +
                  remaining + " second" + (remaining == 1 ? "" : "s") +
                  " remaining)\n");
        return;
    }

    loc_ob = find_object(current_room);
    if(!loc_ob) catch(loc_ob = load_object(current_room));
    if(!loc_ob) {
        if(who) write("EX-5 location error. Contact an admin.\n");
        return;
    }

    tname  = lower_case(target_name);
    target = present(tname, loc_ob);
    if(!target) {
        if(who)
            write("No target matching '" + target_name +
                  "' in the EX-5's current zone.\n");
        return;
    }
    if(!living(target)) {
        if(who)
            write("The EX-5's targeting computer cannot lock a weapon solution\n"
                  "on that. Vehicle weapons require living targets.\n");
        return;
    }

    dmg            = (random(6) + 1 + random(6) + 1) * 10;
    last_fire_time = now;

    tell_room(loc_ob,
        "A searing crack of energy erupts from the EX-5 Behemoth's particle arrays!\n",
        ({}));
    RIFTS_COMBAT_D->apply_vehicle_mdc_damage(target, dmg, PARTICLE_BEAM);
    if(who)
        write("Particle beam fires on " + (string)target->query_cap_name() +
              " for " + dmg + " MDC. Capacitors recharging " +
              WEAPON_COOLDOWN + " seconds.\n");
}
