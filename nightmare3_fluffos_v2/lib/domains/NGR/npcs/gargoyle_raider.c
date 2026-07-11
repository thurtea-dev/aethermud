/* /domains/NGR/npcs/gargoyle_raider.c
   a gargoyle raider. MDC creature. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("gargoyle raider");
    set_id( ({ "gargoyle raider", "gargoyle raider 2", "raider", "gargoyle" }) );
    set_short("a gargoyle raider");
    set_long(
        "A gargoyle of the Empire's raiding castes, lean and fast, hide\n"
        "the grey of wet ash. It crosses no man's land in low, gliding\n"
        "bounds, and it has already seen you.");
    set_race("gargoyle");
    set_level(13);
    set_exp(70000);
    set_alignment(-700);
    set_aggressive(1);
    set_stats("MDC", 150);
    set_stats("max_MDC", 150);
    set_stats("PS", 28);
    set_hp(150);
    set_property("mdc_creature", 1);
}
