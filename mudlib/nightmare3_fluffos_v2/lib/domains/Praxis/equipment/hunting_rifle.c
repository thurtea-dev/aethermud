// /domains/Praxis/equipment/hunting_rifle.c
// Conventional bolt-action hunting rifle -- SDC weapon, no MDC punch.
// Named in occ.c's "wilderness scout" and "tribal warrior" starting_equipment
// text ("hunting rifle or shotgun" / "hunting rifle") but never had an item
// file until this fix. Uses the same e_clip ammo item as every other
// ranged weapon in this lib (see the single-undifferentiated-ammo-type
// finding in the equipment audit -- a separate, deferred architectural
// question, not fixed here).

inherit "/std/weapon";

void create() {
    ::create();
    set_name("hunting rifle");
    set_id( ({ "hunting rifle", "rifle", "bolt-action rifle" }) );
    set_short("a hunting rifle");
    set_long(
        "A conventional bolt-action hunting rifle, well-maintained wood and\n"
        "steel. No match for MDC armor, but reliable against ordinary game\n"
        "and un-armored threats. Damage: 4d6 SDC.");
    set_mass(1200);
    set_value(1500);
    set_wc(10);
    set_ac(0);
    set_type("projectile");
    set_property("mdc_weapon", 0);
    set_property("sdc_weapon", 1);
    set_property("damage_num", 4);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "4d6 SDC");
    set_property("ammo_max", 6);
    set_property("current_ammo", 6);
    set_property("ammo_type", "e_clip");
    set_property("ranged", 1);
}
