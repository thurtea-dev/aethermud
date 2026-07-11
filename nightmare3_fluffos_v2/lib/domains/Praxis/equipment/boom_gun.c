// /domains/Praxis/equipment/boom_gun.c
// Northern Gun boom gun. Heavy rail-style cannon.

inherit "/std/weapon";

void create() {
    ::create();
    set_name("boom gun");
    set_id(({ "boom gun", "boomgun", "rail gun", "cannon" }));
    set_short("a boom gun");
    set_long(
        "A massive Northern Gun boom gun. Shoulder-mounted rail cannon\n"
        "firing hyper-velocity slugs. 1d4x10 MDC per shot. Two-handed.\n"
        "Requires W.P. Heavy Weapons.");
    set_mass(8500);
    set_value(45000);
    set_wc(20);
    set_type("heavy");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 1);
    set_property("damage_sides", 4);
    set_property("damage_bonus", 0);
    set_property("damage_multiplier", 10);
    set_property("damage_string", "1d4x10 MD");
    set_property("rifts_weapon", 1);
    set_property("two_handed", 1);
}
