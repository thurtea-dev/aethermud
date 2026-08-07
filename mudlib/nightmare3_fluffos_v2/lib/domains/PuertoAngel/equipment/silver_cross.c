/* /domains/PuertoAngel/equipment/silver_cross.c
   Silver cross from the abandoned church in Puerto Angel.
   When held and wielded with genuine faith, effective against vampires.
   The vampires of Puerto Angel know this is here. They leave it alone. */

#include <std.h>

inherit "/std/Object";

void create() {
    ::create();
    set_name("silver cross");
    set_id( ({ "silver cross", "cross", "crucifix", "holy cross",
               "old cross", "church cross" }) );
    set_short("a heavy silver cross");
    set_long(
        "A heavy cross cast in old silver, dark with age. The metalwork\n"
        "is expert -- pre-Rifts, maybe pre-electric. A craftsperson made\n"
        "this with the intention of making it last. It radiates no magic\n"
        "that you can detect. It is simply very old silver, shaped with care.\n"
        "The vampires of Puerto Angel leave it in the church. Draw your own\n"
        "conclusions about why.");
    set_mass(300);
    set_value(200);
    set_vendor_type("misc");
    set_property("silver", 1);
    set_property("holy_symbol", 1);
    set_property("vampire_weakness", 1);
}
