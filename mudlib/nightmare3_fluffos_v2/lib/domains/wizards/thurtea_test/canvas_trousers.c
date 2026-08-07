/* /domains/wizards/thurtea_test/canvas_trousers.c
   Cosmetic legs-slot sample for wear testing. No protection. */

#include <std.h>

inherit ARMOUR;

void create() {
    ::create();
    set_name("canvas trousers");
    set_id( ({ "trousers", "canvas trousers", "pants" }) );
    set_short("a pair of canvas trousers");
    set_long(
        "Plain canvas trousers in the same faded tan as a work shirt,\n"
        "with deep cargo pockets. A tag inside the waistband reads\n"
        "SAMPLE: LEGS SLOT. They offer no protection at all.\n");
    set_armour_type("pants");
    set_property("rifts_slot", "legs");
    set_ac(0);
    set_mass(50);
    set_value(10);
}
