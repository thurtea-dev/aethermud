// /domains/Praxis/equipment/cybernetics/cyber_legs.c
// Cybernetic leg prosthetics (pair).

inherit "/std/Object";

void create() {
    ::create();
    set_name("cybernetic legs");
    set_id( ({ "cyber legs", "bionic legs", "prosthetic legs", "cybernetic legs" }) );
    set_short("a pair of cybernetic legs");
    set_long(
        "Reinforced alloy leg prosthetics from hip to foot. Hydraulic actuators\n"
        "deliver sprint speed and crushing kick force. Spd +10, SDC +20.\n"
        "Installed by a qualified cyber-doc.");
    set_mass(0);
    set_value(18000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "legs");
    set_property("bionic_name", "cyber_legs");
    set_property("spd_bonus", 10);
    set_property("sdc_bonus", 20);
}
