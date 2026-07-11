// /domains/Praxis/monsters/gargoyle_raider.c
// Hostile gargoyle raider from the dimensional wilds.

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("gargoyle raider");
    set_id(({ "gargoyle", "raider", "gargoyle raider" }));
    set_short("a gargoyle raider");
    set_long(
        "A stone-skinned gargoyle with leathery wings folded against its back.\n"
        "Its claws gleam and its eyes burn with predatory hunger.");
    set_race("gargoyle");
    set_level(7);
    set_exp(28000);
    set_alignment(-500);
    set_aggressive(1);
    set_stats("MDC", 70);
    set_stats("max_MDC", 70);
    set_stats("PS", 18);
    set_hp(140);
    set_property("can_be_windrush", 1);
}
