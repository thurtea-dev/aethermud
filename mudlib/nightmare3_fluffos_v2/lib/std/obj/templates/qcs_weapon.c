/* QCS weapon template - placeholders replaced at creation time. */
#include <std.h>

inherit "/std/weapon";

void create() {
    ::create();
    set_name("QCS_NAME");
    set_id( ({ "QCS_ID", "weapon" }) );
    set_short("QCS_SHORT");
    set_long("QCS_LONG");
    set_mass(50);
    set_value(100);
    set_wc(5);
    set_type("blunt");
    set_property("sdc_weapon", 1);
    set_property("mdc_weapon", 0);
    set_property("damage_num", 1);
    set_property("damage_sides", 6);
    set_property("damage_string", "1d6 SDC");
}
