// /d/Praxis/equipment/tool_kit.c
// Mechanic's tool kit -- operator starting gear.

inherit "/std/Object";

void create() {
    ::create();
    set_name("tool kit");
    set_id( ({ "tool kit", "toolkit", "tools", "mechanic kit", "kit" }) );
    set_short("a mechanic's tool kit");
    set_long(
        "A battered case of hand tools, meters, and diagnostic gear.\n"
        "Wrenches, screwdrivers, soldering iron, multimeter, and wire\n"
        "strippers. Everything an operator needs in the field.");
    set_mass(3000);
    set_value(500);
    set_vendor_type("equipment");
}
