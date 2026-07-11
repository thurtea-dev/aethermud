/* /realms/thurtea/area/armor/testarmor.c */
/* QCS armor by thurtea */

/* QCS armor template - placeholders replaced at creation time. */
#include <std.h>

inherit "/std/armour";

void create() {
    ::create();
    set_name("testarmor");
    set_id( ({ "testarmor", "armor" }) );
    set_short("testarmor armor");
    set_long("A newly crafted armor piece awaiting description.");
    set_type("body armour");
    set_limbs( ({ "torso" }) );
    set_ac(3);
    set_property("sdc_armor", 1);
    set_property("mdc_armor", 0);
    set_property("armor_ar", 10);
    set_property("armor_sdc", 20);
    set_property("armor_mdc", 0);
    set_property("current_armor_sdc", 20);
    set_property("current_armor_mdc", 0);
    set_property("rifts_slot", "armor");
}
