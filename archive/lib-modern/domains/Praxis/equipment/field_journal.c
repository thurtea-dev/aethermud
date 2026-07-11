// /d/Praxis/equipment/field_journal.c
// Rogue scholar field journal.

inherit "/std/Object";

void create() {
    ::create();
    set_name("field journal");
    set_id( ({ "field journal", "journal", "notebook", "log book" }) );
    set_short("a battered field journal");
    set_long(
        "Thick paper, waterproof cover. Half filled with observations,\n"
        "sketches, and notes on pre-Rifts ruins. A rogue scholar's most\n"
        "valuable non-weapon possession.");
    set_mass(200);
    set_value(50);
    set_vendor_type("equipment");
}
