// /d/Praxis/equipment/science_kit.c
// Rogue scientist portable field science kit.

inherit "/std/Object";

void create() {
    ::create();
    set_name("science kit");
    set_id( ({ "science kit", "kit", "science gear", "field kit", "lab kit" }) );
    set_short("a portable science kit");
    set_long(
        "Sample vials, test strips, a portable meter, and analysis tools.\n"
        "Compact enough for field research. A rogue scientist's breadwinner\n"
        "for identifying artifacts, chemicals, and biological samples.");
    set_mass(2000);
    set_value(600);
    set_vendor_type("equipment");
}
