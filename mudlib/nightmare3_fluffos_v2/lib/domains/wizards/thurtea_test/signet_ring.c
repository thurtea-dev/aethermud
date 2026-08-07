/* /domains/wizards/thurtea_test/signet_ring.c
   Cosmetic ring-slot sample for wear testing (fills ring1 then
   ring2; buy two to test both). No protection. */

#include <std.h>

inherit ARMOUR;

void create() {
    ::create();
    set_name("signet ring");
    set_id( ({ "ring", "signet ring", "signet" }) );
    set_short("a steel signet ring");
    set_long(
        "A plain steel signet ring, its face engraved with a simple\n"
        "test-pattern grid. Tiny letters inside the band read SAMPLE:\n"
        "RING SLOT. It offers no protection at all.\n");
    set_armour_type("ring");
    set_property("rifts_slot", "ring");
    set_ac(0);
    set_mass(5);
    set_value(10);
}
