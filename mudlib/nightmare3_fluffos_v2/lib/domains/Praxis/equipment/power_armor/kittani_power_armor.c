// /d/Praxis/equipment/power_armor/kittani_power_armor.c
// Kittani combat power armor -- MDC body armor /* approx 200 MDC */
// Wearable armor stub; full power armor logic reserved for future expansion.

inherit "/std/armour";

void create() {
    ::create();
    set_name("Kittani power armor");
    set_id( ({ "kittani power armor", "kittani armor", "power armor", "armor" }) );
    set_short("a suit of Kittani combat power armor");
    set_long(
        "Sleek Kittani power armor in dark chitin-like composite plating,\n"
        "crafted with Splugorth technology.  The suit amplifies the wearer's\n"
        "strength and provides life support.  Faint bioluminescent markings\n"
        "trace the seams in green-gold.  MDC: 200.");
    set_mass(5000);
    set_value(80000);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg",
                  "head" }) );
    set_ac(10);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 200);
    set_property("current_armor_mdc", 200);
    set_property("armor_ar", 16);
    set_property("power_armor", 1);
    set_property("ps_bonus", 20);
}
