/* /domains/wizards/thurtea_test/travel_boots.c
   Cosmetic feet-slot sample for wear testing. No protection. */

#include <std.h>

inherit ARMOUR;

void create() {
    ::create();
    set_name("travel boots");
    set_id( ({ "boots", "travel boots", "boot" }) );
    set_short("a pair of travel boots");
    set_long(
        "Ankle-high travel boots with composite soles and worn laces. A\n"
        "stamp inside one heel reads SAMPLE: FEET SLOT. They offer no\n"
        "protection at all.\n");
    set_armour_type("boots");
    set_property("rifts_slot", "feet");
    set_ac(0);
    set_mass(40);
    set_value(10);
}
