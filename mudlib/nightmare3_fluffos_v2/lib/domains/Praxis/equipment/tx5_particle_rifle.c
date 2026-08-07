// /domains/Praxis/equipment/tx5_particle_rifle.c
// Coalition TX-5 particle beam rifle -- signature SAMAS pilot sidearm.
// Named in occ.c's "cs samas rpa pilot" starting_equipment text but never
// had an item file until this fix.

inherit "/std/weapon";

void create() {
    ::create();
    set_name("TX-5 particle beam rifle");
    set_id( ({ "tx-5", "tx5", "particle beam rifle", "particle rifle",
               "particle beam", "rifle" }) );
    set_short("a TX-5 particle beam rifle");
    set_long(
        "A Coalition particle beam weapon issued to elite SAMAS pilots.\n"
        "Sleek, heavy alloy housing with a distinctive blue-white discharge.\n"
        "Fires 6d6 MD particle bursts. E-clip capacity: 10 shots.");
    set_mass(1600);
    set_value(25000);
    set_wc(18);
    set_ac(0);
    set_type("projectile");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 6);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "6d6 MD");
    set_property("ammo_max", 10);
    set_property("current_ammo", 10);
    set_property("ammo_type", "e_clip");
    set_property("ranged", 1);
    set_property("rifts_weapon", 1);
}
