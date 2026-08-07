// /domains/Praxis/monsters/atlantean_guardian.c
// Stone guardian near Atlantean ruins.

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("atlantean guardian");
    set_id(({ "guardian", "atlantean guardian", "golem", "statue" }));
    set_short("an Atlantean stone guardian");
    set_long(
        "A humanoid statue of blue-green stone animated by ancient magic.\n"
        "Runic channels pulse along its limbs. It moves with terrible purpose.");
    set_race("golem");
    set_level(7);
    set_exp(28000);
    set_alignment(200);
    set_aggressive(1);
    set_stats("MDC", 90);
    set_stats("max_MDC", 90);
    set_hp(140);
}
