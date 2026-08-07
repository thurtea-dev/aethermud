/* /domains/wizards/thurtea_test/field_cap.c
   Cosmetic head-slot sample for wear testing. No protection. */

#include <std.h>

inherit ARMOUR;

void create() {
    ::create();
    set_name("field cap");
    set_id( ({ "cap", "field cap", "hat" }) );
    set_short("a gray field cap");
    set_long(
        "A plain gray field cap with a short brim, the kind issued by\n"
        "half the militias in North America. A small tag inside the band\n"
        "reads SAMPLE: HEAD SLOT. It offers no protection at all.\n");
    set_armour_type("helmet");
    set_property("rifts_slot", "head");
    set_ac(0);
    set_mass(20);
    set_value(10);
}
