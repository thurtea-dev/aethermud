// /d/Praxis/equipment/cp30_laser.c
// Coalition CP-30 Heavy Laser Pistol -- MDC ranged /* approx 3d6 MD, 15 shots */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("CP-30 laser pistol");
    set_id( ({ "cp-30", "cp30", "laser pistol", "heavy pistol", "pistol" }) );
    set_short("a CP-30 heavy laser pistol");
    set_long(
        "A heavy-duty Coalition laser sidearm. Bulkier than standard pistols\n"
        "but delivers significantly more punch per shot. 3d6 MD per shot.\n"
        "E-clip capacity: 15 shots.");
    set_mass(700);
    set_value(6000);
    set_wc(16);
    set_ac(0);
    set_type("projectile");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 3);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "3d6 MD");
    set_property("ammo_max", 15);
    set_property("current_ammo", 15);
    set_property("ammo_type", "e_clip");
    set_property("ranged", 1);
}
