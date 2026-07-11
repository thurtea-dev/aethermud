// /d/Praxis/equipment/e_clip.c
// Standard E-clip -- energy ammunition for Coalition laser weapons.

inherit "/std/Object";

void create() {
    ::create();
    set_name("e-clip");
    set_id( ({ "e-clip", "eclip", "energy clip", "clip", "ammo" }) );
    set_short("a standard e-clip");
    set_long(
        "A standard-capacity Coalition energy clip. Fully charged, it holds\n"
        "enough power for one full magazine of most Coalition sidearms and\n"
        "rifles. The Coalition skull-and-crossbones logo is embossed on\n"
        "the side.");
    set_mass(100);
    set_value(300);
    set_vendor_type("ammo");
    set_property("e_clip", 1);
    set_property("ammo_type", "e_clip");
    set_property("ammo_count", 30);
    set_property("ammo_max", 30);
}
