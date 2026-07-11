/* /domains/Praxis/equipment/magic_items/rune_dagger.c
   Lesser rune dagger. Indestructible black blade covered in runes.
   Standard MDC weapon path: 2d6 MD. */

#include <std.h>

inherit "/std/weapon";

void create() {
    ::create();
    set_name("rune dagger");
    set_id( ({ "rune dagger", "dagger", "black dagger", "rune blade" }) );
    set_short("a black rune dagger");
    set_long(
        "A dagger forged in one piece from dull black metal, its blade and\n"
        "grip covered in flowing runes that glimmer when firelight touches\n"
        "them. Lesser rune weapons are indestructible and strike as mega-\n"
        "damage weapons. This one hums very softly, as if asleep.");
    set_mass(400);
    set_value(45000);
    set_wc(10);
    set_type("knife");
    set_property("mdc_weapon", 1);
    set_property("rune_weapon", 1);
    set_property("indestructible", 1);
    set_property("damage_num", 2);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "2d6 MD (rune blade)");
}
