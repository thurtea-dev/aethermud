// /domains/demon_plane/equipment/demon_ichor_vial.c
// Minor drop from demon sliver stalkers.

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("demon ichor vial");
    set_id(({ "demon ichor vial", "ichor vial", "vial", "ichor" }));
    set_short("a vial of demon ichor");
    set_long(
        "Thick black ichor in a salvaged CS sample vial. Alchemists and\n"
        "techno-wizards sometimes pay for demon fluids. Handle with gloves.");
    set_mass(50);
    set_value(200);
}
