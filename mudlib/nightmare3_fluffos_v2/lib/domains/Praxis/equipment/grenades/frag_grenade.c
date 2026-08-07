// /d/Praxis/equipment/grenades/frag_grenade.c
// Standard fragmentation grenade -- SDC explosive.

inherit "/std/Object";

void create() {
    ::create();
    set_name("fragmentation grenade");
    set_id( ({ "fragmentation grenade", "frag grenade", "grenade", "frag" }) );
    set_short("a fragmentation grenade");
    set_long(
        "A standard pre-Rifts fragmentation grenade, still effective against\n"
        "unarmored targets. Pin-and-lever design. Green anodized body with a\n"
        "yellow band marking the explosive fill. SDC weapon only: useless\n"
        "against MDC beings or armor.");
    set_mass(450);
    set_value(50);
    set_vendor_type("weapon");
    set_property("explosive", 1);
    set_property("sdc_explosive", 1);
    set_property("blast_radius", 20);
    set_property("damage_type", "SDC");
    set_property("blast_damage", "5d6");
}
