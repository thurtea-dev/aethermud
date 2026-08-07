/* /d/Praxis/monsters/magic_bush.c
   Animated thornbush found in the New Camelot court garden.
   Level 2 SDC creature. Defends if struck. Non-aggressive until threatened. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("bush");
    set_id( ({ "bush", "magic bush", "thornbush", "animated bush", "plant" }) );
    set_short("a gnarled magic thornbush");
    set_long(
        "A thornbush about chest height, with gnarled woody stems and leaves\n"
        "that shimmer with a faint silver sheen. The thorns on its branches\n"
        "are long and hard as iron. It sways slightly even though the air\n"
        "is still. The shimmer pulses slowly: inhale, exhale.\n"
        "It is alive in a way ordinary plants are not.\n"
    );

    set_race("plant");
    set_class("beast");
    set_level(2);
    set_exp(600);
    set_alignment(0);
    set_aggressive(0);
    set_moving(0);

    set_stats("IQ", 2);
    set_stats("ME", 5);
    set_stats("MA", 2);
    set_stats("PS", 8);
    set_stats("PP", 6);
    set_stats("PE", 12);
    set_stats("PB", 1);
    set_stats("Spd", 3);

    set_stats("SDC", 25);
    set_stats("max_SDC", 25);
    set_stats("rifts_hp", 10);
    set_stats("max_rifts_hp", 10);
}

int query_attack_damage() {
    return random(4) + 1;
}
