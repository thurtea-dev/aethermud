/* /domains/Praxis/equipment/ghostly_katana.c
   Translucent katana that phases through armor.
   Does MDC to MDC beings, SDC to SDC beings, bypasses armor entirely.
   Cannot be seen by undead enemies. */

#include <std.h>
#include <daemons.h>
#include <move.h>

inherit WEAPON;

void create() {
    ::create();
    set_name("katana");
    set_id( ({ "katana", "ghostly katana", "ghostly katana of soul slaying",
               "translucent katana", "phasing katana", "soul slaying katana" }) );
    set_short("a translucent katana");
    set_long(
        "A katana that seems to phase in and out of existence. The blade is translucent,\n"
        "barely visible, like a heat shimmer. It passes through physical matter with\n"
        "unnatural ease. Undead beings cannot perceive it at all.\n"
    );
    set_mass(150);
    set_wc(14);
    set_type("blade");
    set_property("weapon_type", "blade");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 3);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "3d6 MD");
    set_property("armor_bypass", 1);
    set_property("invisible_to_undead", 1);
    set_property("magical", 1);
    set_value(0);
}

int move(mixed dest) {
    int ret;

    ret = ::move(dest);
    if(ret == MOVE_OK && environment(this_object()) &&
       userp(environment(this_object())) &&
       !creatorp(environment(this_object())))
        catch(UNIQUE_ITEMS_D->mark_taken("ghostly_katana"));
    return ret;
}
