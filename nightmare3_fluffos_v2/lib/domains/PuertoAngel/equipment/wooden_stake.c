/* /domains/PuertoAngel/equipment/wooden_stake.c
   A sharpened wooden stake. Folklore says it works. Puerto Angel's
   vampires are old enough that folklore may not be enough. */

#include <std.h>

inherit WEAPON;

void create() {
    ::create();
    set_name("stake");
    set_id( ({ "stake", "wooden stake", "sharpened stake" }) );
    set_short("a sharpened wooden stake");
    set_long(
        "A length of hardwood, whittled to a point and dark with handling.\n"
        "Simple. Old-fashioned. The kind of thing every vampire story agrees\n"
        "on, for whatever that is worth against something old enough to have\n"
        "outlived the stories.\n"
    );
    set_mass(20);
    set_value(30);
    set_wc(4);
    set_type("blade");
    set_property("weapon_type", "blade");
    set_property("mdc_weapon", 0);
    set_property("damage_num", 1);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "1d6 SDC");
    set_property("stake", 1);
    set_property("wood", 1);
    set_property("vampire_weakness", 1);
}
