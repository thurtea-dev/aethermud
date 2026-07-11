// /d/Praxis/equipment/medical_bag.c
// Body fixer / cyber-doc medical bag.

inherit "/std/Object";

void create() {
    ::create();
    set_name("medical bag");
    set_id( ({ "medical bag", "med bag", "doctor bag", "bag", "medikit" }) );
    set_short("a medical bag");
    set_long(
        "Bandages, antiseptics, splints, and basic surgical tools.\n"
        "Scalpels, hemostats, suture kit, and field-grade painkillers.\n"
        "The standard kit for a body fixer working in the field.");
    set_mass(2500);
    set_value(800);
    set_vendor_type("medical");
}
