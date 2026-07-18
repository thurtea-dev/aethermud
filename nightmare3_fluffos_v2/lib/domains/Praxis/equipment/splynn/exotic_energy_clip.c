// /d/Praxis/equipment/splynn/exotic_energy_clip.c
// Double-capacity Kittani energy clip from the Splynn market.

inherit "/std/Object";

void create() {
    ::create();
    set_name("exotic energy clip");
    set_id( ({ "exotic energy clip", "exotic clip", "kittani clip", "double clip", "clip" }) );
    set_short("an exotic double-capacity e-clip");
    set_long(
        "A Kittani-manufactured energy clip finished in black anodized metal\n"
        "with alien script along the side. It holds twice the charge of a\n"
        "standard Coalition e-clip (60 shots instead of 30) and is\n"
        "compatible with most standard energy weapons. The Kittani script\n"
        "translates roughly as 'premium grade'. Someone paid attention.");
    set_mass(180);
    set_value(800);
    set_vendor_type("ammo");
    set_property("e_clip", 1);
    set_property("ammo_type", "e_clip");
    set_property("ammo_count", 60);
    set_property("ammo_max", 60);
    set_property("exotic_clip", 1);
}
