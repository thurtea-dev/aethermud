/* /domains/Tolkeen/monsters/cs_siege_scout.c
   Aggressive CS scout on the siege edge. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("CS siege scout");
    set_id( ({ "cs siege scout", "siege scout", "scout", "coalition",
               "soldier" }) );
    set_short("a CS siege scout");
    set_long(
        "A Coalition scout in light armor, binoculars and a laser rifle.\n"
        "Marking Tolkeen's walls for the next push.");

    set_race("human");
    set_class("fighter");
    set_level(6);
    set_exp(18000);
    set_alignment(-300);
    set_aggressive(1);
    set_gender("male");
    set_body_type("human");

    set_stats("IQ", 12);
    set_stats("ME", 10);
    set_stats("MA", 8);
    set_stats("PS", 14);
    set_stats("PP", 15);
    set_stats("PE", 14);
    set_stats("PB", 9);
    set_stats("Spd", 16);

    set_max_hp(45);
    set_hp(45);
    set_stats("MDC", 60);
    set_stats("max_MDC", 60);
    set_stats("is_MDC", 1);
    set_property("mdc_creature", 1);
    set_stats("rifts_hp", 45);
    set_stats("max_rifts_hp", 45);

    set_die( (: "on_death" :) );
}

void on_death(object killer) {
    if(!killer || !objectp(killer)) killer = this_player();
    if(!killer || !objectp(killer)) return;
    if(!(int)QUEST_D->is_quest_active(killer, "silence_the_scout")) return;
    QUEST_D->update_objective(killer, "silence_the_scout", "scout_silenced", 1);
    QUEST_D->complete_quest(killer, "silence_the_scout");
    tell_object(killer,
        "The scout goes down before it can radio in. Tolkeen's walls stay "
        "unmarked, for now.\n");
}
