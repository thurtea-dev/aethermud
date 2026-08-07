/* /domains/Praxis/equipment/cybernetics/irmss_kit.c
   IRMSS internal robot medical surgeon system. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("IRMSS medical system");
    set_id( ({ "irmss", "irmss medical system", "irmss kit",
        "medical system", "robot medical system" }) );
    set_short("an IRMSS medical system");
    set_long(
        "An Internal Robot Medical Surgeon System: a swarm of micro-machines\n"
        "seeded through the bloodstream that clot wounds, knit tissue and\n"
        "stabilize the dying automatically. Installed by cyber-doc.");
    set_mass(0);
    set_value(25000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "internal_medical");
    set_property("bionic_name", "irmss_medical");
}
