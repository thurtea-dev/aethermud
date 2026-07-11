/* /domains/NGR/equipment/triax_helmet.c
   Triax PA-09 power armor helmet. NGR military issue. MDC: 40. */

inherit "/std/armour";

void create() {
    ::create();
    set_name("Triax PA-09 helmet");
    set_id( ({ "triax pa-09 helmet", "pa-09 helmet", "triax helmet",
               "helmet", "combat helmet" }) );
    set_short("a Triax PA-09 helmet");
    set_long(
        "A sealed combat helmet from the Triax PA-09 power armor series. NGR\n"
        "military issue. The visor is polarized for glare and flash suppression.\n"
        "Internal HUD displays are dark but the mounting points are intact.\n"
        "The seals are good: this helmet will hold atmosphere. MDC: 40.\n"
    );
    set_mass(600);
    set_value(3200);
    set_type("head armour");
    set_limbs( ({ "head" }) );
    set_ac(4);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 40);
    set_property("current_armor_mdc", 40);
    set_property("armor_ar", 13);
}
