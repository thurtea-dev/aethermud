/* /domains/NewCamelot/npcs/camelot_guide.c
   Gamemaster NPC, New Camelot flavor. All chargen-help mechanics (status,
   help topics, greet) are inherited from chargen_guide.c unchanged; only
   the appearance text is reflavored so it fits the gatehouse instead of
   Praxis. Added 2026-07-21 for parity with chitown_start.c's guide. */

#include <std.h>

inherit "/domains/Praxis/npcs/chargen_guide";

void create() {
    ::create();
    set_short("a Rifts Gamemaster");
    set_long(
        "A calm figure in a herald's tabard worn over practical traveling\n"
        "gear, a data slate tucked under one arm despite the medieval\n"
        "trappings around them. They can answer questions about character\n"
        "creation if asked. Say 'help creation', 'status', or 'what next'.");
    set_property("position_str", "waits patiently near the archway.");
}
