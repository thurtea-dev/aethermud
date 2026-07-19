/* /domains/wizards/thurtea_test/sparring_drone.c
   Low-threat legacy-stat target: plain NM3 hp pool, no MDC and no
   Rifts pools, so damage falls through to the legacy whole-body
   fallback path. Passive; attacks only when attacked. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("sparring drone");
    set_id( ({ "drone", "sparring drone", "target" }) );
    set_short("a salvaged sparring drone");
    set_long(
        "A pre-Rifts industrial robot rebuilt as a practice target. Its\n"
        "plating is ordinary steel, dented from years of testing, and its\n"
        "single arm swings a padded baton in slow, predictable arcs. A\n"
        "stenciled label on its chassis reads TARGET UNIT 1: LEGACY.\n");
    set_race("android");
    set_level(2);
    set_exp(50);
    set_alignment(0);
    set_aggressive(0);
    set_max_hp(120);
    set_hp(120);
    set_wimpy(0);
    set_property("position_str", "idles in a slow patrol circle.");
}
