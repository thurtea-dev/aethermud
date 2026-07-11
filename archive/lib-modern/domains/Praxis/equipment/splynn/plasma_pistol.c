/* /d/Praxis/equipment/splynn/plasma_pistol.c
   High-output plasma pistol sold in the Splynn black market.
   6d6 MDC, 400ft range, 10 shots. */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("plasma pistol");
    set_id( ({ "plasma pistol", "plasma", "pistol", "plasma gun" }) );
    set_short("a plasma pistol");
    set_long(
        "A compact plasma pistol of non-Coalition manufacture. The barrel\n"
        "glows faintly blue when charged. Fires a pulse of superheated plasma\n"
        "capable of punching through most MDC armor. 6d6 MD per shot.\n"
        "E-clip capacity: 10 shots. Range: 400 ft.");
    set_mass(900);
    set_value(25000);
    set_wc(22);
    set_ac(0);
    set_type("projectile");
    set_property("mdc_weapon",       1);
    set_property("damage_num",       6);
    set_property("damage_sides",     6);
    set_property("damage_bonus",     0);
    set_property("damage_string",    "6d6 MD");
    set_property("shots_max",        10);
    set_property("shots_remaining",  10);
    set_property("ranged",           1);
    set_property("range_feet",       400);
    set_property("no_sell",          1);
}
