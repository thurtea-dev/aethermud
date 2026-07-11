// /d/Praxis/equipment/dead_boy_armor.c
// Coalition Dead Boy Armor (CA-4) -- MDC body armor /* 80 MDC */

inherit "/std/armour";

void create() {
    ::create();
    set_name("Dead Boy armor");
    set_id( ({ "dead boy armor", "dead boy", "ca-4", "ca4",
               "coalition armor", "body armor", "armor" }) );
    set_short("a suit of Coalition Dead Boy armor");
    set_long(
        "Full-body CA-4 Coalition environmental armor in matte black. The\n"
        "distinctive skull-faced helmet and armored bodysuit of the feared\n"
        "Coalition Dead Boys. MDC-rated plasteel construction.\n"
        "MDC: 80.");
    set_mass(2500);
    set_value(15000);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg" }) );
    set_ac(10);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 80);
    set_property("current_armor_mdc", 80);
    set_property("armor_ar", 15);
}
