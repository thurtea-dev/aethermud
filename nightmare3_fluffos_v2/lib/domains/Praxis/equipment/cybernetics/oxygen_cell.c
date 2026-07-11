/* /domains/Praxis/equipment/cybernetics/oxygen_cell.c
   Oxygen storage cell implant. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("oxygen storage cell");
    set_id( ({ "oxygen storage cell", "oxygen cell", "o2 cell",
        "storage cell", "cell" }) );
    set_short("an oxygen storage cell");
    set_long(
        "A compressed oxygen reservoir that plumbs into the lungs, good for\n"
        "thirty minutes of air underwater, in smoke or in thin atmosphere.\n"
        "Installed by cyber-doc.");
    set_mass(0);
    set_value(2500);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "torso_internal");
    set_property("bionic_name", "oxygen_storage_cell");
}
