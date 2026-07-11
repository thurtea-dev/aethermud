/* /domains/Praxis/monsters/alvurron_rift_wisp.c
   a rift wisp. MDC creature. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("rift wisp");
    set_id( ({ "rift wisp", "wisp" }) );
    set_short("a rift wisp");
    set_long(
        "A ribbon of pale light that did not finish becoming something.\n"
        "It drifts in slow figure-eights beneath the crack in the air,\n"
        "and when it senses you it stops, which is worse.");
    set_race("entity");
    set_level(9);
    set_exp(30000);
    set_alignment(-200);
    set_aggressive(1);
    set_stats("MDC", 60);
    set_stats("max_MDC", 60);
    set_stats("PS", 15);
    set_hp(60);
    set_property("mdc_creature", 1);
}
