/* /d/Praxis/equipment/ammo/e_clip_long.c
   Long 60-shot energy clip. Bulkier but double capacity. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("long e-clip");
    set_id( ({ "long e-clip", "long clip", "e-clip", "eclip", "clip", "ammo" }) );
    set_short("a long e-clip");
    set_long(
        "A high-capacity energy clip. Holds 60 shots -- twice the standard\n"
        "capacity, at the cost of extra bulk and weight. Used by soldiers who\n"
        "expect sustained firefights.");
    set_mass(180);
    set_value(90);
    set_vendor_type("ammo");
    set_property("e_clip", 1);
    set_property("ammo_type", "e_clip");
    set_property("ammo_count", 60);
    set_property("ammo_max", 60);
}
