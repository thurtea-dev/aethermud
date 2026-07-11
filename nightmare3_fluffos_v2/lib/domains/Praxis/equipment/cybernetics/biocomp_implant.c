// /d/Praxis/equipment/cybernetics/biocomp_implant.c
// Bionic computer implant.

inherit "/std/Object";

void create() {
    ::create();
    set_name("bionic computer implant");
    set_id( ({ "biocomp", "bionic computer", "computer implant", "brain implant",
               "neural computer", "biocomp implant" }) );
    set_short("a bionic computer implant");
    set_long(
        "A neural-linked computer chip surgically implanted in the brain.\n"
        "Enhances cognitive processing, provides perfect recall, and can run\n"
        "simple programs autonomously. IQ bonus: +2. Skill bonus: +15%\n"
        "computer operation. Requires qualified cyber-doc for installation.");
    set_mass(0);
    set_value(5000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "brain_internal");
    set_property("bionic_name", "biocomp_implant");
    set_property("skill_bonus_computer_operation", 15);
    set_property("iq_bonus", 2);
}
