// /d/Praxis/equipment/ngr_rail_gun.c
// NGR assault rail gun -- MDC ranged /* approx 4d6 MD, 30 shots */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("NGR rail gun");
    set_id( ({ "ngr rail gun", "rail gun", "railgun", "ngr rifle", "rifle" }) );
    set_short("an NGR assault rail gun");
    set_long(
        "A heavy-caliber electromagnetic rail gun issued to NGR infantry.\n"
        "Fires tungsten-alloy penetrators capable of punching through Brodkil\n"
        "and Gargoyle armor.  4d6 MD per burst.  30-round magazine.");
    set_mass(4000);
    set_value(18000);
    set_wc(22);
    set_ac(0);
    set_type("projectile");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 4);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "4d6 MD");
    set_property("ammo_max", 30);
    set_property("current_ammo", 30);
    set_property("ammo_type", "e_clip");
    set_property("ranged", 1);
    set_property("two_handed", 1);
}
