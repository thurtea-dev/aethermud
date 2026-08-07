/* /domains/NewCamelot/npcs/lesser_fire_elemental.c
   a lesser fire elemental. MDC creature. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("lesser fire elemental");
    set_id( ({ "lesser fire elemental", "fire elemental", "elemental" }) );
    set_short("a lesser fire elemental");
    set_long(
        "A man-sized whirl of flame bound inside the summoning circle,\n"
        "compressed into a shape that is legs and arms only by courtesy.\n"
        "It seethes against the wards, radiating heat and a truly\n"
        "impressive sense of grievance.");
    set_race("elemental");
    set_level(8);
    set_exp(25000);
    set_alignment(-400);
    set_aggressive(0);
    set_stats("MDC", 90);
    set_stats("max_MDC", 90);
    set_stats("PS", 20);
    set_hp(90);
    set_property("mdc_creature", 1);
}
