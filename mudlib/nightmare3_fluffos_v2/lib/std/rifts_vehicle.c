// /std/rifts_vehicle.c
// Base vehicle object for Rifts MUD.
// Inherits CONTAINER so living beings can move inside.
// Players drive via the _drive.c command; enter/exit via actions here.

#include <std.h>
#include <vehicle.h>

inherit CONTAINER;

private int __fuel;
private int __max_fuel;
private int __speed;
private int __mdc;
private int __max_mdc;

void create() {
    ::create();
    set_property(PROP_IS_VEHICLE, 1);
    set_name("vehicle");
    set_id(({ "vehicle" }));
    set_short("a vehicle");
    set_long("A vehicle.");
    set_mass(5000);
    set_max_encumbrance(3000);
    __fuel     = 20;
    __max_fuel = 20;
    __speed    = 1;
    __mdc      = 40;
    __max_mdc  = 40;
}

int query_fuel()     { return __fuel; }
int query_max_fuel() { return __max_fuel; }
int query_speed()    { return __speed; }
int query_mdc()      { return __mdc; }
int query_max_mdc()  { return __max_mdc; }

void set_fuel(int v)     { __fuel     = v; }
void set_max_fuel(int v) { __max_fuel = v; }
void set_speed(int v)    { __speed    = v; }
void set_mdc(int v)      { __mdc      = v; }
void set_max_mdc(int v)  { __max_mdc  = v; }

varargs int receive_objects(object ob) {
    if(!ob) ob = previous_object();
    return 1;   // accept all objects including living beings
}

int cmd_enter(string str) {
    if(this_player()->move(this_object())) {
        write("You can't get into that.\n");
        return 1;
    }
    write("You climb into " + query_short() + ".\n");
    tell_room(environment(this_object()),
        (string)this_player()->query_cap_name() +
        " climbs into " + query_short() + ".\n",
        ({ this_player() }));
    return 1;
}

int cmd_exit(string str) {
    object dest;

    dest = environment(this_object());
    if(!dest) {
        write("There is nowhere to exit to.\n");
        return 1;
    }
    if(this_player()->move(dest)) {
        write("You can't get out.\n");
        return 1;
    }
    write("You climb out of " + query_short() + ".\n");
    tell_room(dest,
        (string)this_player()->query_cap_name() +
        " climbs out of " + query_short() + ".\n",
        ({ this_player() }));
    return 1;
}

void vehicle_move(string dest) {
    object *pass;
    int i;

    __fuel--;
    move(dest);
    pass = all_inventory(this_object());
    i    = sizeof(pass);
    while(i--) {
        if(living(pass[i]))
            catch(pass[i]->describe_current_room(1));
    }
}

void init() {
    ::init();
    if(environment(this_player()) == this_object()) {
        add_action("cmd_exit", "exit");
        add_action("cmd_exit", "leave");
    } else {
        add_action("cmd_enter", "enter");
        add_action("cmd_enter", "board");
    }
}
