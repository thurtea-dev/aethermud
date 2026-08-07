/* /domains/Horton/equipment/salvaged_armor_plate.c
   Salvaged Coalition Dead Boy armor plate. SDC 50, AR 10. */

#include <std.h>

inherit "/std/armour";

void create() {
    ::create();
    set_name("salvaged armor plate");
    set_id( ({ "salvaged armor plate", "armor plate", "salvaged plate",
               "dead boy plate", "body armor", "armor" }) );
    set_short("a salvaged armor plate");
    set_long(
        "A section of Coalition Dead Boy armor, cracked and repainted but\n"
        "still functional. Someone stripped it from a battlefield and made it\n"
        "work again. Armor Rating 10, SDC 50.");
    set_mass(1200);
    set_value(1800);
    set_type("body armour");
    set_limbs( ({ "torso" }) );
    set_ac(4);
    set_property("rifts_slot", "armor");
    set_property("sdc_armor", 1);
    set_property("armor_ar", 10);
    set_property("armor_sdc", 50);
    set_property("current_armor_sdc", 50);
}
