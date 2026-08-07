// /d/Praxis/equipment/ngr_body_armor.c
// New German Republic infantry body armor -- MDC /* approx 80 MDC */

inherit "/std/armour";

void create() {
    ::create();
    set_name("NGR body armor");
    set_id( ({ "ngr body armor", "ngr armor", "german armor", "body armor", "armor" }) );
    set_short("a suit of NGR infantry body armor");
    set_long(
        "Standard-issue New German Republic infantry armor.  Gray-green in color\n"
        "with the NGR eagle stamped on the left pauldron.  Sealed against\n"
        "chemical attack with a built-in filter mask and HUD visor.  MDC: 80.");
    set_mass(1900);
    set_value(4500);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg" }) );
    set_ac(6);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 80);
    set_property("current_armor_mdc", 80);
    set_property("armor_ar", 14);
}
