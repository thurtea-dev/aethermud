// /d/Praxis/equipment/ppe_battery.c
// PPE battery -- stores 30 PPE, used by techno-wizards and wand-bearers.

inherit "/std/Object";

void create() {
    ::create();
    set_name("PPE battery");
    set_id( ({ "ppe battery", "battery", "power battery", "magical battery" }) );
    set_short("a PPE battery");
    set_long(
        "A smooth crystalline cylinder about the size of a soda can, warm to\n"
        "the touch. A faint luminescence pulses inside it at irregular intervals.\n"
        "This battery was charged at a ley line nexus and stores 30 PPE for\n"
        "use by techno-wizards, magic items, or wands.");
    set_mass(200);
    set_value(500);
    set_vendor_type("magical");
    set_property("ppe_battery", 1);
    set_property("ppe_stored", 30);
    set_property("ppe_max", 30);
}
