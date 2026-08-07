/* /domains/Praxis/equipment/rebreather.c
   Rebreather mask: underwater breathing gear. Worn in the neck slot
   (the mask seals over mouth and nose; the harness rides at the
   throat). The ocean-descent gate in the Splynn ocean rooms checks
   for this item worn, the Swim as a Fish spell, or an installed
   oxygen cell or cyber lung. Cosmetic slot: carries no protection. */

#include <std.h>

inherit ARMOUR;

void create() {
    ::create();
    set_name("rebreather");
    set_id( ({ "rebreather", "rebreather mask", "mask",
               "breathing mask" }) );
    set_short("a rebreather mask");
    set_long(
        "A compact breathing mask of molded polymer and treated rubber.\n"
        "It seals over the mouth and nose, and a squat recycler unit at\n"
        "the throat scrubs and recirculates air for hours of underwater\n"
        "operation. The head strap is stamped with a Northern Gun stock\n"
        "number. Wear it to breathe below the surface.\n");
    set_armour_type("necklace");
    set_property("rifts_slot", "neck");
    set_property("rebreather", 1);
    set_ac(0);
    set_mass(30);
    set_value(1200);
}
