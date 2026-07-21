// /domains/NewCamelot/equipment/knight_shield.c
// Knight's shield, worn on back slot, grants +2 AR when worn.

inherit "/std/armour";

void create() {
    ::create();
    set_name("knight shield");
    set_id( ({ "knight shield", "shield", "kite shield" }) );
    set_short("a knight's shield");
    set_long(
        "A large kite shield bearing the heraldic colors of New Camelot.\n"
        "Steel-rimmed and sturdy. Grants +2 to Armor Rating when carried.\n"
        "SDC: 40.");
    set_mass(1200);
    set_value(500);
    set_type("shield");
    set_limbs( ({ "back" }) );
    set_ac(2);
    set_property("rifts_slot", "back");
    set_property("sdc_armor", 1);
    set_property("armor_ar", 2);
    set_property("armor_sdc", 40);
    set_property("current_armor_sdc", 40);
    set_property("shield", 1);
    set_property("ar_bonus", 2);
}
