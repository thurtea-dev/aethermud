/* /d/Praxis/vehicles/ex5/ex5_cargo.c
   EX-5 Behemoth -- starboard cargo bay.
   Connected east from the main hull. */

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
    set_short("EX-5 Behemoth: Cargo Bay");
    set_long(
        "The starboard cargo bay occupies the right flank of the EX-5.\n"
        "It is a wide open space with tie-down rings welded into the\n"
        "deck at regular intervals. Whatever cargo the EX-5 originally\n"
        "carried is long gone. Empty cargo nets hang from overhead rails.\n\n"
        "Along the inboard wall are four modular storage lockers, all\n"
        "empty and open. The deck has seen heavy use: scratches, gouges,\n"
        "and the outline of crates that were dragged across it decades ago.\n\n"
        "A personnel airlock on the outer hull wall is sealed but\n"
        "functional. It provides a second exit from the EX-5 when\n"
        "the vehicle is deployed. Currently locked to interior access only.\n\n"
        "Exit: west to the main hull.");
    set_exits( ([
        "west" : "/domains/Praxis/vehicles/ex5/ex5_hull"
    ]) );
    set_listen("default",
        "Empty cargo space. The nets creak slightly against the overhead\n"
        "rails as the machine settles.");
    set_smell("default",
        "Dust, old packing material, and the faint ghost of whatever\n"
        "cargo this bay used to carry.");
    set_items( ([
        "rings"   : "Cargo tie-down rings welded into the deck. Heavy-gauge.\n"
                    "They could secure vehicles, supplies, or anything else\n"
                    "the EX-5 was tasked with transporting.",
        "nets"    : "Empty cargo nets hanging from overhead rails. They have\n"
                    "not been used in a long time.",
        "lockers" : "Four modular wall lockers, standing open and empty.\n"
                    "Standard military field issue, built to survive combat.",
        "airlock" : "A personnel airlock on the outer hull wall. The controls\n"
                    "are functional but locked to interior access only while\n"
                    "the EX-5 is powered down.",
        "deck"    : "The cargo bay deck. Scratched and gouged from decades of\n"
                    "heavy loads. The outline of crate positions is still faintly\n"
                    "visible in the worn metal.",
        "crates"  : "The crates are long gone. Only their outline remains in\n"
                    "the worn deck surface."
    ]) );
}

void reset() {
    ::reset();
}
