// /domains/chitown/equipment/fake_cs_id.c
// Forged Coalition States identification card.

inherit "/std/object";

void create() {
    ::create();
    set_name("fake CS ID card");
    set_id( ({ "cs id", "id card", "identification", "fake id", "fake cs id",
               "coalition id", "card" }) );
    set_short("a forged CS ID card");
    set_long(
        "A convincing forgery of a Coalition States citizen identification\n"
        "card. The holographic CS skull emblem catches the light correctly.\n"
        "The name and biometrics are fabricated but the card will pass a\n"
        "casual check. A skilled inspector would spot the inconsistencies.");
    set_mass(10);
    set_value(100);
    set_property("fake_cs_id", 1);
    set_property("salvage", 0);
}
