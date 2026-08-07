// /d/Praxis/equipment/grenades/smoke_grenade.c
// Smoke grenade -- area denial, no damage.

inherit "/std/Object";

void create() {
    ::create();
    set_name("smoke grenade");
    set_id( ({ "smoke grenade", "smoke", "grenade" }) );
    set_short("a smoke grenade");
    set_long(
        "A grey cylindrical smoke grenade. Creates a 20-foot diameter cloud\n"
        "of dense smoke on detonation, blocking line of sight for both ranged\n"
        "attacks and targeting systems. Duration: four melees outdoors,\n"
        "approximately one minute indoors. No damage, -4 to all ranged attacks\n"
        "within the affected area.");
    set_mass(400);
    set_value(30);
    set_vendor_type("weapon");
    set_property("explosive", 1);
    set_property("smoke_effect", 1);
    set_property("blast_radius", 20);
    set_property("ranged_penalty", 4);
}
