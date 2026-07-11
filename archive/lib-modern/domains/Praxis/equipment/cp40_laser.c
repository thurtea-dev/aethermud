// /d/Praxis/equipment/cp40_laser.c
// Coalition CP-40 Pulse Laser Rifle -- MDC ranged /* approx 2d6 MD, 20 shots */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("CP-40 laser rifle");
    set_id( ({ "cp-40", "cp40", "laser rifle", "pulse laser", "rifle" }) );
    set_short("a CP-40 pulse laser rifle");
    set_long(
        "The standard Coalition infantry weapon. Matte black alloy housing\n"
        "with Coalition skull markings. Fires 2d6 MD pulse bursts. E-clip\n"
        "capacity: 20 shots.");
    set_mass(1100);
    set_value(8000);
    set_wc(14);
    set_ac(0);
    set_type("projectile");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 2);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "2d6 MD");
    set_property("ammo_max", 20);
    set_property("current_ammo", 20);
    set_property("ammo_type", "e_clip");
    set_property("ranged", 1);
}
