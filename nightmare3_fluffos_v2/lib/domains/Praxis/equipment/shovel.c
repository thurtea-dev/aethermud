// /d/Praxis/equipment/shovel.c
// A heavy entrenching shovel -- required for the dig command in Splynn Market.

inherit "/std/weapon";

void create() {
    ::create();
    set_name("shovel");
    set_id( ({ "shovel", "entrenching shovel", "entrenching tool", "spade" }) );
    set_short("a heavy entrenching shovel");
    set_long(
        "A military-grade entrenching shovel with a folding handle locked\n"
        "in the extended position. Heavy-duty steel blade, rubber-grip handle.\n"
        "Useful for digging, and in a pinch, for hitting things.\n"
        "Damage as improvised weapon: 1d4 SDC.");
    set_mass(800);
    set_value(150);
    set_wc(3);
    set_ac(0);
    set_type("club");
    set_property("mdc_weapon", 0);
    set_property("sdc_weapon", 1);
    set_property("damage_num", 1);
    set_property("damage_sides", 4);
    set_property("damage_bonus", 0);
    set_property("damage_string", "1d4 SDC");
    set_property("is_shovel", 1);
}
