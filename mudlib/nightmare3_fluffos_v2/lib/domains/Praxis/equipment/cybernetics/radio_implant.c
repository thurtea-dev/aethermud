/* /domains/Praxis/equipment/cybernetics/radio_implant.c
   Built-in radio receiver and transmitter. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("radio implant");
    set_id( ({ "radio implant", "radio receiver", "radio transmitter",
        "implant radio", "radio" }) );
    set_short("a radio implant");
    set_long(
        "A subdermal short-wave radio wired to the inner ear and jaw.\n"
        "Receives and transmits on standard bands out to five miles.\n"
        "Installed by cyber-doc.");
    set_mass(0);
    set_value(5200);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "ear_internal");
    set_property("bionic_name", "radio_implant");
}
