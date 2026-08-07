// /d/Praxis/equipment/knife.c
// Standard knife -- SDC melee weapon /* approx 1d6 SDC */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("knife");
    set_id( ({ "knife", "combat knife", "blade" }) );
    set_short("a combat knife");
    set_long(
        "A standard steel combat knife, sturdy and well-balanced. Not MDC-\n"
        "capable, but effective against ordinary targets. Damage: 1d6 SDC.");
    set_mass(200);
    set_value(80);
    set_wc(5);
    set_ac(0);
    set_type("knife");
    set_property("mdc_weapon", 0);
    set_property("sdc_weapon", 1);
    set_property("damage_num", 1);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "1d6 SDC");
}
