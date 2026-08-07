// /d/Praxis/equipment/chain_mail.c
// Chain mail -- SDC armor, AR 14 /* approx AR 14, SDC 60 */

inherit "/std/armour";

void create() {
    ::create();
    set_name("chain mail");
    set_id( ({ "chain mail", "chainmail", "chain", "mail" }) );
    set_short("a suit of chain mail");
    set_long(
        "Interlocked steel rings forming a full haubergeon. Heavier than\n"
        "leather but considerably more effective at stopping blades and\n"
        "blows. Not MDC-rated. Armor Rating 14, SDC 60.");
    set_mass(2000);
    set_value(600);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg" }) );
    set_ac(5);
    set_property("rifts_slot", "armor");
    set_property("sdc_armor", 1);
    set_property("armor_ar", 14);
    set_property("armor_sdc", 60);
    set_property("current_armor_sdc", 60);
}
