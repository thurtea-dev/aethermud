// /d/Praxis/equipment/splynn/predator_armor.c
// Predator body armor with optical camouflage system.
// MDC: 80. Active camo reduces visibility while worn.

inherit "/std/armour";

void create() {
    ::create();
    set_name("predator armor");
    set_id( ({ "predator armor", "predator", "camo armor", "optical armor",
               "armor", "body armor" }) );
    set_short("a suit of predator armor");
    set_long(
        "Black body armor with an integrated optical camouflage system:\n"
        "a network of micro-lenses and phase-shift emitters that bends\n"
        "light around the wearer. The effect is not true invisibility\n"
        "but it substantially reduces visual detection at range and in\n"
        "low light. The armor itself is rated at 80 MDC. The optics\n"
        "run on a power cell good for six hours of continuous use.\n\n"
        "It is not Coalition manufacture. The serial number has been removed.");
    set_mass(2000);
    set_value(3500);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg" }) );
    set_ac(7);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 80);
    set_property("current_armor_mdc", 80);
    set_property("armor_ar", 14);
    set_property("rifts_armor", 1);
    set_property("is_invisible", 1);
    set_property("optical_camo", 1);
}
