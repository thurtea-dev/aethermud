/* /domains/Horton/monsters/quarry_lurker.c
   Something that nests in the old quarry. Level 7, SDC 90. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("quarry lurker");
    set_id( ({ "quarry lurker", "lurker", "beast", "animal" }) );
    set_short("a quarry lurker");
    set_long(
        "A low, mottled predator with too many joints in its forelimbs.\n"
        "It hugs the quarry walls and drops on anything that climbs down\n"
        "looking for scrap. Eyes reflect like wet glass.\n"
    );

    set_race("animal");
    set_class("beast");
    set_level(7);
    set_alignment(-250);
    set_aggressive(1);
    set_gender("neutral");
    set_body_type("animal");

    set_stats("IQ", 6);
    set_stats("ME", 8);
    set_stats("MA", 4);
    set_stats("PS", 17);
    set_stats("PP", 16);
    set_stats("PE", 15);
    set_stats("PB", 2);
    set_stats("Spd", 18);

    set_max_hp(55);
    set_hp(55);
    set_stats("SDC", 90);
    set_stats("max_SDC", 90);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 55);
    set_stats("max_rifts_hp", 55);

    set_die( (: "on_death" :) );
}

void on_death(object killer) {
    if(!killer || !objectp(killer)) killer = this_player();
    if(!killer || !objectp(killer)) return;
    if(!(int)QUEST_D->is_quest_active(killer, "quarry_lurker_cull")) return;
    QUEST_D->update_objective(killer, "quarry_lurker_cull", "lurker_defeated", 1);
    QUEST_D->complete_quest(killer, "quarry_lurker_cull");
    tell_object(killer,
        "The lurker stops moving. The quarry rim is quiet for the first "
        "time in a while.\n");
}

int query_attack_damage() {
    return random(7) + 5;
}
