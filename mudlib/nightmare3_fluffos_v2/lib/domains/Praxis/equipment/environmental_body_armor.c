// /d/Praxis/equipment/environmental_body_armor.c
// Environmental Body Armor (EBA) -- MDC body armor /* 90 MDC */

inherit "/std/armour";

void create() {
    ::create();
    set_name("environmental body armor");
    set_id( ({ "eba", "environmental body armor", "environmental armor",
               "body armor", "armor" }) );
    set_short("a suit of environmental body armor");
    set_long(
        "A common MDC body armor suit found throughout the Rifts world.\n"
        "Fully sealed and environmental, with a built-in air supply and\n"
        "heads-up display visor. The workhorse armor of mercenaries and\n"
        "adventurers everywhere. MDC: 90.");
    set_mass(1800);
    set_value(5000);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg" }) );
    set_ac(7);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 90);
    set_property("current_armor_mdc", 90);
    set_property("armor_ar", 13);
}
