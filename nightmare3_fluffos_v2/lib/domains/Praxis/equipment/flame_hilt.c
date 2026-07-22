/* /domains/Praxis/equipment/flame_hilt.c
   Metal hilt. Charge with PPE to manifest a blade of pure flame for 5 minutes.

   Renewable unique: at most one hilt loose in the world at a time, but
   unlike the Sword of Atlantis it comes back after a cooldown rather
   than being gone forever. UNIQUE_ITEMS_D's query_taken_within("flame_hilt",
   FLAME_HILT_COOLDOWN) is checked by each spawn site before cloning a
   new one; mark_taken() below (mirroring sword_of_atlantis.c's move()
   override) starts that cooldown on first mortal pickup. Cooldown
   length: see FLAME_HILT_COOLDOWN, defined identically at each spawn
   site (hydra_lair.c, collapsed_building.c, forest_lake_far_shore.c,
   chitown_hydra_treasure.c) since none of them inherit this file. */

#include <std.h>
#include <daemons.h>
#include <move.h>

inherit WEAPON;

private int __charged;

void create() {
    ::create();
    set_name("hilt");
    set_id( ({ "hilt", "flame hilt", "metal hilt" }) );
    set_short("a metal hilt");
    set_long(
        "A metal hilt with no blade. The grip is wrapped in worn leather. "
        "Faint runes are etched along the crossguard. Charge it with PPE by typing "
        "'charge hilt' to manifest a blade of crackling flame.\n"
    );
    set_mass(200);
    set_wc(3);
    set_type("blade");
    set_property("weapon_type", "blade");
    set_property("mdc_weapon", 0);
    set_property("damage_num", 1);
    set_property("damage_sides", 4);
    set_property("damage_bonus", 0);
    set_property("damage_string", "1d4 SDC");
    set_value(0);
    __charged = 0;
}

int query_charged() { return __charged; }

/* Start the world cooldown on first mortal pickup. Creators handling
   it for tests do not burn the flag (matches sword_of_atlantis.c). */
int move(mixed dest) {
    int ret;

    ret = ::move(dest);
    if(ret == MOVE_OK && environment(this_object()) &&
       userp(environment(this_object())) &&
       !creatorp(environment(this_object())))
        catch(UNIQUE_ITEMS_D->mark_taken("flame_hilt"));
    return ret;
}

int cmd_charge(string str) {
    int ppe;
    object bearer;

    if(!str || strsrch(lower_case(str), "hilt") == -1) return 0;
    bearer = environment(this_object());
    if(!bearer || !living(bearer)) {
        write("You are not holding the hilt.\n");
        return 1;
    }
    if(__charged) {
        tell_object(bearer, "The hilt is already charged. A flame blade burns from it.\n");
        return 1;
    }
    ppe = (int)bearer->query_stats("PPE");
    if(ppe < 10) {
        tell_object(bearer, "You need at least 10 PPE to charge the hilt.\n");
        return 1;
    }
    bearer->set_stats("PPE", ppe - 10);
    __charged = 1;
    set_wc(14);
    set_property("damage_num", 4);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "4d6 MD");
    set_short("a flame-bladed hilt");
    set_property("mdc_weapon", 1);
    tell_object(bearer,
        "You pour PPE into the hilt. A blade of crackling flame erupts from it.\n");
    tell_room(environment(bearer),
        (string)bearer->query_cap_name() + " ignites a blade of flame from a metal hilt.\n",
        ({ bearer }));
    call_out("extinguish_blade", 300, bearer);
    return 1;
}

void extinguish_blade(object bearer) {
    __charged = 0;
    set_wc(3);
    set_property("damage_num", 1);
    set_property("damage_sides", 4);
    set_property("damage_bonus", 0);
    set_property("damage_string", "1d4 SDC");
    set_short("a metal hilt");
    set_property("mdc_weapon", 0);
    if(objectp(bearer))
        tell_object(bearer,
            "The flame blade gutters and vanishes. The hilt is inert again.\n");
}

void init() {
    ::init();
    add_action("cmd_charge", "charge");
}
