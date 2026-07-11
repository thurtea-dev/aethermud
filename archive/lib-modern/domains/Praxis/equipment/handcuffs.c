// /d/Praxis/equipment/handcuffs.c
// Restraint gear for headhunters and bounty hunters.

inherit "/std/Object";

void create() {
    ::create();
    set_name("handcuffs");
    set_id( ({ "handcuffs", "cuffs", "restraints", "binders" }) );
    set_short("a pair of handcuffs");
    set_long(
        "Heavy-duty carbon steel handcuffs. Standard bounty-hunter issue.\n"
        "Ratchet-lock mechanism, key-release. Not easily defeated by\n"
        "ordinary human-level strength.");
    set_mass(300);
    set_value(150);
    set_vendor_type("equipment");
}
