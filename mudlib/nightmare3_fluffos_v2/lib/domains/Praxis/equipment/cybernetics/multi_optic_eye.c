/* /domains/Praxis/equipment/cybernetics/multi_optic_eye.c
   Multi-optic cybernetic eye. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("multi-optic eye");
    set_id( ({ "multi-optic eye", "multi optic eye", "multi-optic",
        "optic eye", "eye" }) );
    set_short("a multi-optic eye");
    set_long(
        "A jeweled cybernetic eye packed with lens systems: telescopic,\n"
        "macro, passive nightvision and thermal-imaging filters.\n"
        "Installed by cyber-doc. Replaces one natural eye.");
    set_mass(0);
    set_value(6000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "eye");
    set_property("bionic_name", "multi_optic_eye");
}
