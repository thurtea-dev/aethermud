/* /realms/thurtea/area/weap/testsword.c */
/* QCS weapon by thurtea */

/* QCS weapon template - placeholders replaced at creation time. */
#include <std.h>

inherit "/std/weapon";

void create() {
    ::create();
    set_name("testsword");
    set_id( ({ "testsword", "weapon" }) );
    set_short("a testsword");
    set_long("A newly forged weapon awaiting description.");
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
