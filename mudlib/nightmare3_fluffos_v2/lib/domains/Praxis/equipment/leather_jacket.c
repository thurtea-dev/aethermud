// /d/Praxis/equipment/leather_jacket.c
// Leather jacket -- SDC armor, AR 10 /* AR 10, SDC 30 */

inherit "/std/armour";

void create() {
    ::create();
    set_name("leather jacket");
    set_id( ({ "leather jacket", "jacket", "leather" }) );
    set_short("a leather jacket");
    set_long(
        "A tough leather jacket reinforced at the shoulders and elbows.\n"
        "Not MDC-rated, but provides some protection against SDC attacks.\n"
        "Armor Rating 10, SDC 30.");
    set_mass(800);
    set_value(200);
    set_type("jacket");
    set_limbs( ({ "torso", "left arm", "right arm" }) );
    set_ac(2);
    set_property("rifts_slot", "armor");
    set_property("sdc_armor", 1);
    set_property("armor_ar", 10);
    set_property("armor_sdc", 30);
    set_property("current_armor_sdc", 30);
}
