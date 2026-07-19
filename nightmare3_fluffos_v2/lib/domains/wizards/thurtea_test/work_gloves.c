/* /domains/wizards/thurtea_test/work_gloves.c
   Cosmetic hands-slot sample for wear testing. No protection. */

#include <std.h>

inherit ARMOUR;

void create() {
    ::create();
    set_name("work gloves");
    set_id( ({ "gloves", "work gloves", "glove" }) );
    set_short("a pair of work gloves");
    set_long(
        "Thick leather work gloves, palm-worn and supple from use. A\n"
        "stamp on one cuff reads SAMPLE: HANDS SLOT. They offer no\n"
        "protection at all.\n");
    set_armour_type("gloves");
    set_property("rifts_slot", "hands");
    set_ac(0);
    set_mass(20);
    set_value(10);
}
