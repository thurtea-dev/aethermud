/* /domains/Praxis/equipment/obsidian_blade.c
   Volcanic obsidian blade. Extremely sharp, ignores half of target AR. */

#include <std.h>

inherit WEAPON;

void create() {
    ::create();
    set_name("blade");
    set_id( ({ "blade", "obsidian blade", "obsidian", "volcanic blade" }) );
    set_short("a jagged obsidian blade");
    set_long(
        "A jagged blade of volcanic obsidian, sharp beyond natural explanation.\n"
        "The surface is matte black and seems to absorb light rather than reflect it.\n"
        "Examining it in darkness is impossible. It is unnaturally cold to the touch.\n"
    );
    set_mass(300);
    set_wc(12);
    set_type("blade");
    set_property("weapon_type", "blade");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 4);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "4d6 MD");
    set_property("ar_bypass_half", 1);
    set_property("magical", 1);
    set_value(0);
}
