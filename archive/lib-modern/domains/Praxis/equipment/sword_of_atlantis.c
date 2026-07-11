/* /domains/Praxis/equipment/sword_of_atlantis.c
   The Sword of Atlantis: ancient True Atlantean holy weapon.
   5d6 MD, bypasses AR, +3 strike, +2 parry, unsellable. */

#include <std.h>

inherit WEAPON;

void create() {
    ::create();
    set_name("sword");
    set_id( ({ "sword", "sword of atlantis", "atlantean sword",
               "glowing sword", "ancient sword" }) );
    set_short("the Sword of Atlantis");
    set_long(
        "A longsword of polished Atlantean steel inlaid with glowing blue runes.\n"
        "The blade is etched with the True Atlantean mark of the Warrior Champion.\n"
        "It hums faintly with dimensional energy and feels perfectly balanced.\n"
        "True Atlanteans are said to gain additional power when wielding this\n"
        "weapon against the forces of the Splugorth.\n"
    );
    set_mass(200);
    set_wc(16);
    set_type("blade");
    set_property("weapon_type", "blade");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 5);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "5d6 MD");
    set_property("penetrate_ar", 1);
    set_property("magical", 1);
    set_property("bonus_strike", 3);
    set_property("bonus_parry", 2);
    set_property("no_sell", 1);
    set_value(0);
}
