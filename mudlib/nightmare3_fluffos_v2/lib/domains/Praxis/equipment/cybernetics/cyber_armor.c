/* /domains/Praxis/equipment/cybernetics/cyber_armor.c
   Subdermal cyber-armor plating. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("cyber-armor plating");
    set_id( ({ "cyber-armor", "cyber armor", "cyber-armor plating",
        "armor plating", "plating" }) );
    set_short("cyber-armor plating");
    set_long(
        "Thin composite plates implanted beneath the skin of the torso.\n"
        "Turns knives and small-caliber rounds: +50 SDC. Popular with\n"
        "City Rats who cannot afford full conversion. Installed by cyber-doc.");
    set_mass(0);
    set_value(22000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "torso_armor");
    set_property("bionic_name", "cyber_armor");
}
