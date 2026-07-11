// /d/Praxis/equipment/cybernetics/cyber_arm_right.c
// Cybernetic right arm prosthetic.

inherit "/std/Object";

void create() {
    ::create();
    set_name("cybernetic right arm");
    set_id( ({ "cybernetic arm", "cyber arm", "bionic arm", "right arm",
               "prosthetic arm", "cyber right arm" }) );
    set_short("a cybernetic right arm");
    set_long(
        "A full prosthetic arm from shoulder to fingertips. Military-grade\n"
        "alloy construction with articulated fingers. Stronger than flesh,\n"
        "capable of crushing MDC-level force. Fully articulated for fine work.\n"
        "MDC: 60. PS bonus: +8 to Robotic PS. Installed by qualified cyber-doc.");
    set_mass(0);
    set_value(12000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "right_arm");
    set_property("bionic_name", "cyber_arm_right");
    set_property("mdc_limb", 60);
    set_property("ps_arm_bonus", 8);
}
