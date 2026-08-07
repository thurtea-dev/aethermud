// /d/Praxis/equipment/coalition_grunt_armor.c
// Coalition Grunt Armor (CA-1) -- MDC body armor /* 50 MDC */

inherit "/std/armour";

void create() {
    ::create();
    set_name("Coalition grunt armor");
    set_id( ({ "grunt armor", "ca-1", "ca1", "coalition grunt armor",
               "coalition armor", "body armor", "armor" }) );
    set_short("a suit of Coalition grunt armor");
    set_long(
        "Standard-issue Coalition CA-1 field armor. Lighter than Dead Boy\n"
        "armor but still fully environmental. Worn by regular CS infantry\n"
        "and support troops throughout the Coalition States.\n"
        "MDC: 50.");
    set_mass(2000);
    set_value(8000);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg" }) );
    set_ac(8);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 50);
    set_property("current_armor_mdc", 50);
    set_property("armor_ar", 12);
}
