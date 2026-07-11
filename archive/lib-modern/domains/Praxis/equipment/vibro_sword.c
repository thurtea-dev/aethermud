// /d/Praxis/equipment/vibro_sword.c
// Rifts vibro-sword -- MDC melee weapon /* approx 2d6 MD */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("vibro-sword");
    set_id( ({ "vibro-sword", "vibro sword", "sword" }) );
    set_short("a vibro-sword");
    set_long(
        "A long vibroblade crackling with contained energy. The hilt is\n"
        "reinforced plasteel; the blade is MDC-rated molecular steel.\n"
        "Damage: 2d6 MD.");
    set_mass(600);
    set_value(1200);
    set_wc(12);
    set_ac(1);
    set_type("blade");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 2);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "2d6 MD");
}
