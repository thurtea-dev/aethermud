// /domains/Praxis/equipment/cybernetics/cyber_arm_left.c
// Cybernetic left arm prosthetic.

inherit "/std/Object";

void create() {
    ::create();
    set_name("cybernetic left arm");
    set_id( ({ "cybernetic arm", "cyber arm", "bionic arm", "left arm",
               "prosthetic arm", "cyber left arm" }) );
    set_short("a cybernetic left arm");
    set_long(
        "A full prosthetic arm from shoulder to fingertips. Military-grade\n"
        "alloy construction with articulated fingers. Stronger than flesh.\n"
        "MDC: 60. PS bonus: +8 to Robotic PS. Installed by qualified cyber-doc.");
    set_mass(0);
    set_value(12000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "left_arm");
    set_property("bionic_name", "cyber_arm_left");
    set_property("mdc_limb", 60);
    set_property("ps_arm_bonus", 8);
}
