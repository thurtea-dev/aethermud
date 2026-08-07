// /domains/Praxis/equipment/leather_glove.c
// A heavy falconry glove. Required to trade for a trained hawk from the falconer.

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("glove");
    set_id( ({ "glove", "leather glove", "falconry glove", "gauntlet" }) );
    set_short("a heavy leather glove");
    set_long(
        "A heavy leather falconry glove reinforced at the wrist and forearm.\n"
        "Pre-Rifts make, well-maintained. A trained hawk could perch on this\n"
        "without tearing your arm apart. Sold at the general store for 100 credits.\n"
    );
    set_mass(150);
    set_value(100);
    set_property("is_leather_glove", 1);
}
