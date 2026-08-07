/* /domains/wizards/thurtea_test/canvas_shirt.c
   Cosmetic shirt-slot sample for wear testing. No protection. */

#include <std.h>

inherit ARMOUR;

void create() {
    ::create();
    set_name("canvas shirt");
    set_id( ({ "shirt", "canvas shirt", "work shirt" }) );
    set_short("a canvas work shirt");
    set_long(
        "A sturdy canvas work shirt in faded tan, with two chest pockets\n"
        "and reinforced elbows. A small tag inside the collar reads\n"
        "SAMPLE: SHIRT SLOT. It offers no protection at all.\n");
    set_armour_type("shirt");
    set_property("rifts_slot", "shirt");
    set_ac(0);
    set_mass(40);
    set_value(10);
}
