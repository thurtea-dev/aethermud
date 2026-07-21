/* /domains/NewCamelot/equipment/camelot_longsword.c
   New Camelot pattern longsword: medieval form, MDC alloy edge. */

#include <std.h>

inherit "/std/weapon";

void create() {
    ::create();
    set_name("camelot longsword");
    set_id( ({ "camelot longsword", "longsword", "long sword", "sword" }) );
    set_short("a Camelot longsword");
    set_long(
        "A hand-and-a-half longsword in the classic medieval pattern,\n"
        "forged around a mega-damage alloy core in the New Camelot\n"
        "fashion. The white dragon of the Order is stamped at the base\n"
        "of the blade. 2d6 MD.");
    set_mass(1200);
    set_value(9000);
    set_wc(10);
    set_type("blade");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 2);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "2d6 MD");
}
