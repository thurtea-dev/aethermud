// /d/Praxis/equipment/short_sword.c
// Short sword -- SDC melee weapon /* approx 1d6+1 SDC */

inherit "/std/weapon";

void create() {
    ::create();
    set_name("short sword");
    set_id( ({ "short sword", "shortsword", "sword" }) );
    set_short("a short sword");
    set_long(
        "A dependable single-edged short sword of ordinary steel. Pre-Rifts\n"
        "quality construction, solid if unspectacular. Not MDC-capable.\n"
        "Damage: 1d6+1 SDC.");
    set_mass(450);
    set_value(150);
    set_wc(7);
    set_ac(1);
    set_type("blade");
    set_property("mdc_weapon", 0);
    set_property("sdc_weapon", 1);
    set_property("damage_num", 1);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 1);
    set_property("damage_string", "1d6+1 SDC");
}
