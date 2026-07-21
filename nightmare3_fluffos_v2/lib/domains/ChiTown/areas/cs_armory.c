// /domains/ChiTown/areas/cs_armory.c
// CS Armory. Coalition gear requisition depot.

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/ChiTown/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Chi-Town, CS Armory");
    set_long(
        "Racks of Coalition weapons and gear fill this hardened depot from\n"
        "floor to ceiling. Laser rifles sit in locked cradles. Body armor\n"
        "hangs in numbered rows behind reinforced glass. A requisition counter\n"
        "runs along the near wall beneath CS inventory screens.\n\n"
        "Try 'list' to see available goods. The command plaza is east.");
    set_exits( ([
        "east" : "/domains/ChiTown/areas/cs_command_plaza"
    ]) );
    set_listen("default",
        "Inventory scanners beeping and the clatter of gear on metal racks.");
    set_smell("default",
        "Gun oil, new polymer, and packaged rations.");
    set_items( ([
        "racks" : "Floor-to-ceiling racks of Coalition-standard weapons and gear.",
        "rifles" : "CP-series laser rifles in locked cradles.",
        "armor" : "Dead Boy armor in numbered rows behind reinforced glass.",
        "counter" : "A requisition counter with CS inventory screens."
    ]) );
}

void reset() {
    ::reset();
    if(!present("cs quartermaster", this_object()))
        clone_object(NPC_PATH + "cs_quartermaster")->move(this_object());
}
