/* /d/Praxis/vehicles/ex5/ex5_portside.c
   EX-5 Behemoth -- portside (port/left side) utility compartment.
   Connected west from the main hull. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_property("vehicle_interior", 1);
    set_property("vehicle_daemon", "/daemon/ex5_d");
    set_short("EX-5 Behemoth: Portside Compartment");
    set_long(
        "The portside utility compartment runs the length of the EX-5's\n"
        "left flank. Armor maintenance rigs are bolted to the outer hull\n"
        "wall: articulated arms and spray-seal applicators for field MDC\n"
        "armor repair. A diagnostics panel covers most of the inboard wall,\n"
        "its status indicators all dark.\n\n"
        "A narrow viewport runs along the outer hull at eye level, giving\n"
        "a view of whatever terrain the EX-5 is currently parked in.\n"
        "The view through it is the same as looking out from the outside.\n\n"
        "Exit: east to the main hull.");
    set_exits( ([
        "east" : "/domains/Praxis/vehicles/ex5/ex5_hull"
    ]) );
    set_listen("default",
        "The deep hum of the hull structure and the faint servo noise\n"
        "from the maintenance arms as they settle.");
    set_smell("default",
        "MDC sealant compound and machine oil. The portside gear\n"
        "is more recently maintained than the rest of the vehicle.");
    set_items( ([
        "arms"       : "Articulated maintenance arms for applying MDC sealant to\n"
                       "the outer armor plates. They can reach the full outer skin\n"
                       "from inside without EVA.",
        "rigs"       : "The armor maintenance rigs. Heavy industrial equipment,\n"
                       "designed to keep the EX-5 combat-effective in the field.",
        "diagnostics": "A large diagnostics panel covering the inboard wall. Status\n"
                       "lights all dark. The systems it monitors are offline.",
        "panel"      : "See 'diagnostics'.",
        "viewport"   : "A long narrow armored viewport running at eye level along\n"
                       "the outer hull wall. The view outside shows wherever the\n"
                       "EX-5 is currently parked.",
        "hull"       : "The inner face of the EX-5's portside armor plating. MDC-rated\n"
                       "alloy, thick enough to stop most anti-armor rounds."
    ]) );
}

void reset() {
    ::reset();
}
