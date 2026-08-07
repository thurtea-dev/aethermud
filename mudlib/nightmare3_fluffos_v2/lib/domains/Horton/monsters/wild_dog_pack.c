/* /domains/Horton/monsters/wild_dog_pack.c
   Aggressive wild dog pack on the wilderness trail. Level 4, SDC 60. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("wild dog pack");
    set_id( ({ "wild dog pack", "dog pack", "dogs", "pack", "wild dogs",
               "animal" }) );
    set_short("a pack of wild dogs");
    set_long(
        "Three lean mutts working as a unit, ribs visible through matted fur.\n"
        "They circle with low growls, testing distance and flanking angles.\n"
        "Whatever domestication their ancestors had is long gone. These dogs\n"
        "hunt the trail for anything slow or alone.\n"
    );

    set_race("animal");
    set_class("beast");
    set_level(4);
    set_alignment(-100);
    set_aggressive(1);
    set_gender("neutral");
    set_body_type("animal");

    set_stats("IQ", 5);
    set_stats("ME", 8);
    set_stats("MA", 4);
    set_stats("PS", 14);
    set_stats("PP", 14);
    set_stats("PE", 14);
    set_stats("PB", 3);
    set_stats("Spd", 20);

    set_max_hp(30);
    set_hp(30);
    set_stats("SDC", 60);
    set_stats("max_SDC", 60);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 30);
    set_stats("max_rifts_hp", 30);
}

int query_attack_damage() {
    return random(4) + 3;
}
