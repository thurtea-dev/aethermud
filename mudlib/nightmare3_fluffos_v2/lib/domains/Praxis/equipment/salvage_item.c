// /d/Praxis/equipment/salvage_item.c
// Pre-Rifts salvage -- generic junk worth selling to vendors.
// Clone multiple copies; each gets a random value set on create().

inherit "/std/Object";

private int __sell_value;

void create() {
    ::create();
    __sell_value = 50 + random(151);
    set_name("salvage");
    set_id( ({ "salvage", "pre-rifts salvage", "junk", "scrap" }) );
    set_short("pre-Rifts salvage");
    set_long(
        "A miscellaneous chunk of pre-Rifts technology: circuit boards,\n"
        "corroded wiring, and unidentifiable metal components pressed into a\n"
        "roughly box-shaped bundle. It has no obvious use, but someone in\n"
        "the black market will pay for the raw materials.\n"
        "Value: approx " + __sell_value + " credits.");
    set_mass(400);
    set_value(__sell_value);
    set_vendor_type("salvage");
}
