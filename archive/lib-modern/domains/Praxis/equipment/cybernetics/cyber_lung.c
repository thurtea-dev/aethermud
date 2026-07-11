// /domains/Praxis/equipment/cybernetics/cyber_lung.c
// Cybernetic lung implant.

inherit "/std/Object";

void create() {
    ::create();
    set_name("cybernetic lung");
    set_id( ({ "cyber lung", "bionic lung", "lung implant", "cybernetic lungs" }) );
    set_short("a cybernetic lung implant");
    set_long(
        "A synthetic lung module that filters toxins and boosts oxygen uptake.\n"
        "PE +2. Immunity to most airborne toxins. Installed by cyber-doc.");
    set_mass(0);
    set_value(8000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "lung");
    set_property("bionic_name", "cyber_lung");
    set_property("pe_bonus", 2);
}
