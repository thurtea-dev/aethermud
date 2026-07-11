// /d/Praxis/equipment/spell_components.c
// Techno-wizard spell components pouch.

inherit "/std/Object";

void create() {
    ::create();
    set_name("spell components");
    set_id( ({ "spell components", "components", "pouch", "component pouch" }) );
    set_short("a pouch of spell components");
    set_long(
        "Various crystals, powders, and focus items for techno-wizard work.\n"
        "Quartz shards, copper wire, essence vials, and PPE conductive\n"
        "material. Required for most techno-wizard rituals.");
    set_mass(400);
    set_value(200);
    set_vendor_type("magical");
}
