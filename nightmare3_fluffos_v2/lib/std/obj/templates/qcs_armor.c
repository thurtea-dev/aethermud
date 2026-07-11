/* QCS armor template - placeholders replaced at creation time. */
#include <std.h>

inherit "/std/armour";

void create() {
    ::create();
    set_name("QCS_NAME");
    set_id( ({ "QCS_ID", "armor" }) );
    set_short("QCS_SHORT");
    set_long("QCS_LONG");
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
