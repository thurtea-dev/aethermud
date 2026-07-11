// /d/Praxis/equipment/kittani_laser.c
// Kittani laser rifle -- MDC ranged /* approx 3d6+3 MD, 20 shots */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("Kittani laser rifle");
    set_id( ({ "kittani laser rifle", "kittani laser", "kittani rifle",
               "laser rifle", "rifle" }) );
    set_short("a Kittani laser rifle");
    set_long(
        "A Kittani-manufactured laser rifle with bioluminescent targeting assists\n"
        "along the barrel.  Compact and accurate, it fires focused energy pulses\n"
        "with a characteristic high-pitched crack.  3d6+3 MD.  20-shot clip.");
    set_mass(3000);
    set_value(25000);
    set_wc(20);
    set_ac(0);
    set_type("projectile");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 3);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 3);
    set_property("damage_string", "3d6+3 MD");
    set_property("ammo_max", 20);
    set_property("current_ammo", 20);
    set_property("ammo_type", "e_clip");
    set_property("ranged", 1);
    set_property("two_handed", 1);
}
