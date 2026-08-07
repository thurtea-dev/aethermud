// /d/Praxis/equipment/c18_pistol.c
// Coalition C-18 Laser Pistol -- MDC ranged /* approx 2d4 MD, 20 shots */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("C-18 laser pistol");
    set_id( ({ "c-18", "c18", "laser pistol", "pistol", "sidearm" }) );
    set_short("a C-18 laser pistol");
    set_long(
        "The standard Coalition sidearm, sleek and reliable. Found on every\n"
        "CS soldier's hip. Lighter than the CP-30, it trades stopping power\n"
        "for speed. 2d4 MD per shot. E-clip capacity: 20 shots.");
    set_mass(450);
    set_value(3500);
    set_wc(10);
    set_ac(0);
    set_type("projectile");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 2);
    set_property("damage_sides", 4);
    set_property("damage_bonus", 0);
    set_property("damage_string", "2d4 MD");
    set_property("ammo_max", 20);
    set_property("current_ammo", 20);
    set_property("ammo_type", "e_clip");
    set_property("ranged", 1);
}
