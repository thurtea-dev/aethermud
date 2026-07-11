// /d/Praxis/equipment/bone_dust.c
// Necromantic focus material.

inherit "/std/Object";

void create() {
    ::create();
    set_name("bone dust");
    set_id( ({ "bone dust", "vial", "bone vial", "dust" }) );
    set_short("a vial of bone dust");
    set_long(
        "Ground bone from a sentient creature, sealed in a dark glass vial.\n"
        "Necromantic focus material. The fine powder holds a faint residue\n"
        "of the original creature's life force.");
    set_mass(100);
    set_value(100);
    set_vendor_type("magical");
}
