/* /domains/newcamelot/npcs/young_dragon.c
   a young white dragon. MDC creature. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("young dragon");
    set_id( ({ "young dragon", "dragon" }) );
    set_short("a young white dragon");
    set_long(
        "A young dragon the size of a cargo truck rests on the tower crown,\n"
        "scales the color of milk glass, wings folded like sails at harbor.\n"
        "One gold eye tracks you with lazy, complete attention. It is sworn\n"
        "to the Order, which is the main reason you are still standing here.");
    set_race("dragon");
    set_level(16);
    set_exp(150000);
    set_alignment(500);
    set_aggressive(0);
    set_stats("MDC", 400);
    set_stats("max_MDC", 400);
    set_stats("PS", 40);
    set_hp(400);
    set_property("mdc_creature", 1);
}
