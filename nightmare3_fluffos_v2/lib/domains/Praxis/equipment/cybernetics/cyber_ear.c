/* /domains/Praxis/equipment/cybernetics/cyber_ear.c
   Amplified cybernetic ear implant. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("cybernetic ear");
    set_id( ({ "cyber ear", "bionic ear", "ear implant", "amplified ear" }) );
    set_short("a cybernetic ear implant");
    set_long(
        "An amplified hearing module with sound filters and wide-band\n"
        "pickup. Hears a whisper across a crowded bar. Installed by cyber-doc.");
    set_mass(0);
    set_value(4000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "ear");
    set_property("bionic_name", "cyber_ear");
}
