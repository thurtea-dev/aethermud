// /d/Praxis/equipment/splynn/net_gun.c
// Entanglement net launcher. Non-lethal. Entangles target on hit.
// Zero MDC damage. Used for capture rather than combat.

inherit "/std/weapon";

void create() {
    ::create();
    set_name("net gun");
    set_id( ({ "net gun", "net", "launcher", "entanglement gun", "capture gun" }) );
    set_short("a net gun");
    set_long(
        "A compact launcher that fires a weighted titanium-strand net at\n"
        "high velocity. The net is conductive: on impact it delivers a\n"
        "low-current pulse that tightens the strands automatically around\n"
        "the target. Standard capture tool used by Splugorth slavers,\n"
        "bounty hunters, and anyone else whose plans require a live subject.\n\n"
        "Damage: 0 MDC. On a successful hit, the target is snared in a\n"
        "magic net (30 MDC). They can still fight but cannot flee until\n"
        "the net is destroyed or expires.\n"
        "Effective range: 30 meters. Net canister holds 3 shots.");
    set_mass(600);
    set_value(800);
    set_wc(4);
    set_ac(0);
    set_type("ranged");
    set_property("mdc_weapon", 0);
    set_property("damage_num", 0);
    set_property("damage_sides", 0);
    set_property("damage_bonus", 0);
    set_property("damage_string", "0 damage (entanglement)");
    set_property("entanglement", 1);
    set_property("rifts_weapon", 1);
}
