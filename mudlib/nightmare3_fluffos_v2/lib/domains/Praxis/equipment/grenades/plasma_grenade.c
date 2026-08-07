// /d/Praxis/equipment/grenades/plasma_grenade.c
// Coalition-issue plasma grenade -- MDC explosive.

inherit "/std/Object";

void create() {
    ::create();
    set_name("plasma grenade");
    set_id( ({ "plasma grenade", "grenade", "plasma", "mdc grenade" }) );
    set_short("a plasma grenade");
    set_long(
        "A Coalition-issue plasma grenade, matte black with a red indicator\n"
        "band. The superheated plasma burst on detonation is effective against\n"
        "both MDC armor and MDC creatures. A ten-foot blast radius makes it\n"
        "dangerous to use in confined spaces or near allies.");
    set_mass(450);
    set_value(200);
    set_vendor_type("weapon");
    set_property("explosive", 1);
    set_property("mdc_explosive", 1);
    set_property("blast_radius", 10);
    set_property("damage_type", "MDC");
    set_property("blast_damage", "5d6");
}
