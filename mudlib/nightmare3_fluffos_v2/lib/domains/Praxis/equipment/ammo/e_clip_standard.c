/* /d/Praxis/equipment/ammo/e_clip_standard.c
   Standard 30-shot energy clip. Compatible with most energy weapons. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("e-clip");
    set_id( ({ "e-clip", "e clip", "eclip", "clip", "standard e-clip", "ammo" }) );
    set_short("an e-clip");
    set_long(
        "A standard energy clip. Holds 30 shots and is compatible with most\n"
        "Coalition and civilian energy weapons. A small charge indicator on\n"
        "the side glows green when full.");
    set_mass(100);
    set_value(50);
    set_vendor_type("ammo");
    set_property("e_clip", 1);
    set_property("ammo_type", "e_clip");
    set_property("ammo_count", 30);
    set_property("ammo_max", 30);
}
