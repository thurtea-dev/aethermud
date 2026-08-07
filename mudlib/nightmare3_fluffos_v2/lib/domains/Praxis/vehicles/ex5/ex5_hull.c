// /d/Praxis/vehicles/ex5/ex5_hull.c
// EX-5 Behemoth -- main hull entry room.
// Entry from splynn_underground; north to forward bay, up to cockpit, south to crew quarters.

#include <std.h>
#include <rooms.h>

#include <daemons.h>

inherit ROOM;

#define EX5_D "/daemon/ex5_d"

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_property("vehicle_interior", 1);
    set_property("vehicle_daemon", "/daemon/ex5_d");
    set_short("EX-5 Behemoth: Main Hull");
    set_long(
        "The cavernous hull of the EX-5 Behemoth. Emergency lighting in\n"
        "amber strips lines the ceiling, casting everything in a warm dim\n"
        "glow. The smell of machine oil and old metal is overwhelming.\n"
        "Massive hydraulic struts line the walls, each one as thick as a\n"
        "tree trunk. Conduit bundles run in organized rows to junction\n"
        "boxes that look like they haven't been touched in decades.\n\n"
        "A ladder assembly on the aft wall leads upward to the cockpit.\n"
        "A wide corridor leads north toward the forward weapons bay.\n"
        "A narrower passage leads south to the crew quarters.\n"
        "Hatches port and starboard open to the portside compartment (west)\n"
        "and the cargo bay (east).\n"
        "The hull access hatch is below you. Type 'out' to return\n"
        "to wherever the EX-5 is currently parked.");
    set_exits( ([
        "out"   : "/domains/Praxis/areas/splynn/splynn_underground",
        "north" : "/domains/Praxis/vehicles/ex5/ex5_forward_bay",
        "south" : "/domains/Praxis/vehicles/ex5/ex5_crew_quarters",
        "up"    : "/domains/Praxis/vehicles/ex5/ex5_cockpit",
        "west"  : "/domains/Praxis/vehicles/ex5/ex5_portside",
        "east"  : "/domains/Praxis/vehicles/ex5/ex5_cargo"
    ]) );
    set_listen("default",
        "The deep hum of dormant systems, and the creak of metal settling.");
    set_smell("default",
        "Heavy machine oil, metal, and the faint chemical smell of old MDC sealant.");
    set_items( ([
        "struts" : "Hydraulic struts the diameter of old-growth trees. Each one is\n"
                     "a structural support for the EX-5's enormous frame.",
        "conduit" : "Bundles of conduit running along the walls. Power, data, and\n"
                     "hydraulic lines feeding every system in the machine.",
        "ladder" : "A heavy-duty ladder assembly welded to the aft wall. It leads\n"
                     "upward through the machine's core to the cockpit level.",
        "hatch" : "The hull access hatch. Heavy MDC alloy, currently open.\n"
                     "Type 'out' to exit the EX-5. Port and starboard hatches lead\n"
                     "to the portside compartment (west) and cargo bay (east).",
        "hatches" : "Two interior hatches, one port (west) and one starboard (east).\n"
                     "Port leads to the portside maintenance compartment.\n"
                     "Starboard leads to the cargo bay.",
        "junction" : "Junction boxes the size of refrigerators, covered in pre-Rifts\n"
                     "technical labels. Everything is systematically organized.",
        "lighting" : "Amber emergency strips. They have probably been on for decades."
    ]) );
}

void reset() {
    ::reset();
}

void init() {
    ::init();
    add_action("hull_lock_cmd", "lock");
    add_action("hull_unlock_cmd", "unlock");
}

int hull_lock_cmd(string str) {
    int pin;

    if(!str || sscanf(str, "%d", pin) != 1) {
        write("Syntax: lock <4-digit pin>\n");
        return 1;
    }
    if(pin < 0 || pin > 9999) {
        write("PIN must be four digits (0000-9999).\n");
        return 1;
    }
    EX5_D->lock_hull(pin);
    write("EX-5 hull access locked with new PIN.\n");
    this_player()->tell_room_living(environment(this_object()),
        this_player(), 0, " secures the EX-5 access hatch.\n");
    return 1;
}

int hull_unlock_cmd(string str) {
    int pin;

    if(!str || sscanf(str, "%d", pin) != 1) {
        write("Syntax: unlock <4-digit pin>\n");
        return 1;
    }
    if((int)EX5_D->try_unlock_hull(pin)) {
        write("EX-5 hull access unlocked.\n");
        return 1;
    }
    write("Incorrect PIN.\n");
    return 1;
}
