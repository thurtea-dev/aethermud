// /domains/tolkeen/equipment/ward_charm.c
// Minor protective charm from Tolkeen crypt vault.

inherit "/std/armour";

void create() {
    ::create();
    set_name("ward charm");
    set_id( ({ "charm", "ward charm", "tolkeen charm" }) );
    set_short("a ward charm");
    set_long(
        "A small techno-wizard charm on a leather cord. Runes flicker when\n"
        "Coalition technology is near. It will not stop a boom gun - but it\n"
        "might spoil a scout's aim.");
    set_mass(20);
    set_value(800);
    set_type("ring");
    set_limbs( ({ "right hand", "left hand", "right arm", "left arm", "torso" }) );
    set_ac(1);
}
