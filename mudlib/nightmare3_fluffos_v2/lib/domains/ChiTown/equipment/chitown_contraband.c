/* /domains/ChiTown/equipment/chitown_contraband.c
   Sealed contraband crate found in the Chi-Town black market. No special
   mechanics this pass. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("chitown contraband");
    set_id( ({ "chitown contraband", "contraband", "crate", "contraband crate",
               "sealed crate" }) );
    set_short("a sealed contraband crate");
    set_long(
        "A Coalition-stamped shipping crate with the markings scraped off.\n"
        "Contents unknown. Worth serious credits to the right buyer or\n"
        "serious jail time to the wrong Coalition patrol.");
    set_mass(150);
    set_value(2500);
}
