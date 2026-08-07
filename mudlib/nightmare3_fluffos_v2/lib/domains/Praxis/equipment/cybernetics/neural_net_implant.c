// /domains/Praxis/equipment/cybernetics/neural_net_implant.c
// Neural net computer interface.

inherit "/std/Object";

void create() {
    ::create();
    set_name("neural net implant");
    set_id( ({ "neural net", "neural implant", "brain jack", "neural interface" }) );
    set_short("a neural net implant");
    set_long(
        "A cranial jack and neural processor wired into the motor cortex.\n"
        "ME +1 for computer operation and vehicle interface. Cyber-doc install.");
    set_mass(0);
    set_value(15000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "brain");
    set_property("bionic_name", "neural_net");
    set_property("me_bonus", 1);
}
