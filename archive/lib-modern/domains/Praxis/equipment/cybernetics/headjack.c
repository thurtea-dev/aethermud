// /d/Praxis/equipment/cybernetics/headjack.c
// Headjack neural interface implant.

inherit "/std/Object";

void create() {
    ::create();
    set_name("headjack");
    set_id( ({ "headjack", "neural interface", "head jack", "neural jack",
               "interface implant", "skull port" }) );
    set_short("a headjack neural interface");
    set_long(
        "A small port at the base of the skull. Allows direct neural interface\n"
        "with compatible computer systems, vehicles, and communications gear.\n"
        "The port is nearly invisible when not in use. Standard military and\n"
        "corporate implant. Skill bonus: +20% computer operation.");
    set_mass(0);
    set_value(2000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "head_internal");
    set_property("bionic_name", "headjack");
    set_property("skill_bonus_computer_operation", 20);
}
