// /domains/NewCamelot/equipment/short_sword.c
// Short sword, 1d8 SDC melee weapon.

inherit "/std/weapon";

void create() {
    ::create();
    set_name("short sword");
    set_id( ({ "short sword", "shortsword", "sword" }) );
    set_short("a short sword");
    set_long(
        "A well-balanced short sword of good steel. Single-edged, with a\n"
        "cross-guard and leather-wrapped grip. Pre-Rifts forging. Not MDC-capable.\n"
        "Damage: 1d8 SDC.");
    set_mass(450);
    set_value(300);
    set_wc(8);
    set_ac(1);
    set_type("blade");
    set_property("sdc_weapon", 1);
    set_property("damage_num", 1);
    set_property("damage_sides", 8);
    set_property("damage_bonus", 0);
    set_property("damage_string", "1d8 SDC");
}
