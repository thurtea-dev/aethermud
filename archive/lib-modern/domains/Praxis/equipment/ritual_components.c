// /d/Praxis/equipment/ritual_components.c
// Shifter ritual components bag.

inherit "/std/Object";

void create() {
    ::create();
    set_name("ritual components");
    set_id( ({ "ritual components", "components", "ritual bag", "components bag" }) );
    set_short("a bag of ritual components");
    set_long(
        "Chalk, candles, binding materials, and dimensional focus crystals.\n"
        "Used to draw summoning circles and stabilize rift openings.\n"
        "A shifter's essential toolkit for dimensional work.");
    set_mass(600);
    set_value(300);
    set_vendor_type("magical");
}
