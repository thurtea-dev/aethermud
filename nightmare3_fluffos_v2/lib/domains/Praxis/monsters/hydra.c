// /d/Praxis/monsters/hydra.c
// Three-headed hydra guarding the hydra lair west of the nexus approach.
// MDC: 200. 3 bite attacks per round (3d6 MDC each).
// can_be_windrush=1 allows windrush spell to push it.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("hydra");
    set_id( ({ "hydra", "three-headed hydra", "creature", "beast" }) );
    set_short("a massive three-headed hydra");
    set_long(
        "A nightmarish reptilian creature the size of a small building. Three\n"
        "serpentine heads weave and bob on thick necks, each set of jaws\n"
        "capable of taking a person apart in one bite. The scales are dark\n"
        "green, tough as armor plate. The floor around it is littered with\n"
        "bones. It has held this lair for a very long time.\n"
    );

    set_race("hydra");
    set_class("beast");
    set_level(8);
    set_exp(80000);
    set_alignment(-800);
    set_aggressive(1);

    set_stats("IQ", 4);
    set_stats("ME", 8);
    set_stats("MA", 3);
    set_stats("PS", 28);
    set_stats("PP", 10);
    set_stats("PE", 22);
    set_stats("PB", 2);
    set_stats("Spd", 12);

    set_stats("MDC", 200);
    set_stats("max_MDC", 200);
    set_stats("is_MDC", 1);
    set_stats("rifts_hp", 200);
    set_stats("max_rifts_hp", 200);

    set_property("can_be_windrush", 1);
    set_die( (: "on_death" :) );
}

void on_death() {
    tell_room(environment(this_object()),
        "The hydra collapses with a thunderous crash, all three heads going limp.\n");
}

int query_attack_damage() {
    return random(6) + random(6) + random(6) + 3;
}
