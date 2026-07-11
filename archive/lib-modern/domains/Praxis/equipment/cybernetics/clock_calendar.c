/* /domains/Praxis/equipment/cybernetics/clock_calendar.c
   Clock calendar chip implant. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("clock calendar chip");
    set_id( ({ "clock calendar chip", "clock chip", "calendar chip",
        "clock calendar", "chip" }) );
    set_short("a clock calendar chip");
    set_long(
        "A rice-grain chip that ticks off the exact time, date and elapsed\n"
        "intervals directly into the wearer's mind. Installed by cyber-doc.");
    set_mass(0);
    set_value(1000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "internal_clock");
    set_property("bionic_name", "clock_calendar");
}
