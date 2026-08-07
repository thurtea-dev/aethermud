// /domains/Praxis/equipment/c12_laser_rifle.c
// Coalition C-12 heavy assault laser rifle.

inherit "/std/weapon";

void create() {
    ::create();
    set_name("C-12 laser rifle");
    set_id(({ "c-12", "c12", "laser rifle", "assault rifle", "rifle" }));
    set_short("a C-12 heavy assault laser rifle");
    set_long(
        "The standard Coalition heavy infantry rifle. Matte black with CS\n"
        "markings. Fires 3d6 MD bursts. E-clip capacity: 30 shots.");
    set_mass(1400);
    set_value(12000);
    set_wc(16);
    set_type("projectile");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 3);
    set_property("damage_sides", 6);
    set_property("damage_string", "3d6 MD");
    set_property("ammo_max", 30);
    set_property("current_ammo", 30);
    set_property("ammo_type", "e_clip");
    set_property("ranged", 1);
    set_property("rifts_weapon", 1);
}
