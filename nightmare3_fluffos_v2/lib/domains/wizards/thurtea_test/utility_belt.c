/* /domains/wizards/thurtea_test/utility_belt.c
   Cosmetic belt-slot sample for wear testing. No protection. */

#include <std.h>

inherit ARMOUR;

void create() {
    ::create();
    set_name("utility belt");
    set_id( ({ "belt", "utility belt" }) );
    set_short("a scuffed utility belt");
    set_long(
        "A wide leather utility belt with a row of empty tool loops and a\n"
        "heavy steel buckle. A stamp on the underside reads SAMPLE: BELT\n"
        "SLOT. It offers no protection at all.\n");
    set_armour_type("belt");
    set_property("rifts_slot", "belt");
    set_ac(0);
    set_mass(30);
    set_value(10);
}
