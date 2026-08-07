/* /domains/NewCamelot/equipment/dragonfire_lance.c
   Dragonfire lance: treasure of the Camelot Order. MDC weapon. */

#include <std.h>
#include <daemons.h>
#include <move.h>

inherit "/std/weapon";

void create() {
    ::create();
    set_name("dragonfire lance");
    set_id( ({ "dragonfire lance", "lance", "dragon lance" }) );
    set_short("a dragonfire lance");
    set_long(
        "A knight's lance forged in the breath of a living dragon. Heat\n"
        "shimmers along the fluted steel, and the grip is warm even in a\n"
        "cold room. Runes of the Camelot Order spiral down the shaft. It\n"
        "strikes with dragonfire: 4d6 MD.");
    set_mass(1800);
    set_value(60000);
    set_wc(14);
    set_type("polearm");
    set_property("mdc_weapon", 1);
    set_property("magic_item", 1);
    set_property("damage_num", 4);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "4d6 MD (dragonfire)");
}

int move(mixed dest) {
    int ret;

    ret = ::move(dest);
    if(ret == MOVE_OK && environment(this_object()) &&
       userp(environment(this_object())) &&
       !creatorp(environment(this_object())))
        catch(UNIQUE_ITEMS_D->mark_taken("dragonfire_lance"));
    return ret;
}
