/* /domains/Praxis/equipment/cybernetics/targeting_eye.c
   Targeting sight cybernetic eye. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("targeting eye");
    set_id( ({ "targeting eye", "targeting sight", "sight eye", "eye" }) );
    set_short("a targeting sight eye");
    set_long(
        "A matte black cybernetic eye with a crosshair-etched iris. Projects\n"
        "a targeting reticle directly onto the optic nerve: +1 to strike with\n"
        "ranged weapons. Installed by cyber-doc. Replaces one natural eye.");
    set_mass(0);
    set_value(4500);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "eye");
    set_property("bionic_name", "targeting_eye");
}
