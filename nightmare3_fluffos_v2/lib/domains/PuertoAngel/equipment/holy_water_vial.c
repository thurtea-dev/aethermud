/* /domains/PuertoAngel/equipment/holy_water_vial.c
   A small vial of consecrated water from the abandoned church.
   Elena keeps a supply. Whether the consecration still holds after
   all this time is not something anyone in town wants to test. */

#include <std.h>

inherit "/std/Object";

void create() {
    ::create();
    set_name("vial");
    set_id( ({ "vial", "holy water", "holy water vial", "small vial" }) );
    set_short("a vial of holy water");
    set_long(
        "A small glass vial, stoppered with wax, holding water blessed at\n"
        "the church before Puerto Angel changed hands. Meant to be thrown\n"
        "or splashed rather than carried into a fight and hoped with.\n"
    );
    set_mass(5);
    set_value(60);
    set_property("holy", 1);
    set_property("liquid", 1);
    set_property("vampire_weakness", 1);
}
