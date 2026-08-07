// /d/Praxis/equipment/vibro_knife.c
// Rifts vibro-knife -- MDC melee weapon /* approx 1d6 MD */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("vibro-knife");
    set_id( ({ "vibro-knife", "vibro knife", "knife" }) );
    set_short("a vibro-knife");
    set_long(
        "A short vibroblade humming with kinetic energy. The molecularly-\n"
        "aligned edge vibrates at ultrasonic frequencies, shearing through\n"
        "MDC materials. Damage: 1d6 MD.");
    set_mass(200);
    set_value(500);
    set_wc(8);
    set_ac(0);
    set_type("blade");
    set_property("mdc_weapon", 1);
    set_property("damage_num", 1);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "1d6 MD");
    set_property("no_drop", 0);
}
