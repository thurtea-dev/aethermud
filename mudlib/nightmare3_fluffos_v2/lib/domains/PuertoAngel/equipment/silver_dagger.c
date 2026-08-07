/* /domains/PuertoAngel/equipment/silver_dagger.c
   A dagger of worked silver. Elena sells these; the town's vampires
   know exactly what they are and give the buyer a wide berth. */

#include <std.h>

inherit WEAPON;

void create() {
    ::create();
    set_name("dagger");
    set_id( ({ "dagger", "silver dagger" }) );
    set_short("a silver dagger");
    set_long(
        "A dagger forged from worked silver, plain-hilted and honestly made.\n"
        "Not decorative -- this is a working weapon, balanced and sharp. It\n"
        "catches the light in a way that draws attention from anything in\n"
        "town that used to be human.\n"
    );
    set_mass(40);
    set_value(400);
    set_wc(8);
    set_type("blade");
    set_property("weapon_type", "blade");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 2);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "2d6 MD");
    set_property("silver", 1);
    set_property("blade", 1);
    set_property("vampire_weakness", 1);
}
