/* /domains/NGR/npcs/gargoyle_raider.c
   a gargoyle raider. MDC creature. */

#include <std.h>
#include <daemons.h>

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

    set_die( (: "on_death" :) );
}

void on_death(object killer) {
    if(!killer || !objectp(killer)) killer = this_player();
    if(!killer || !objectp(killer)) return;
    if(!(int)QUEST_D->is_quest_active(killer, "border_gargoyle_raider")) return;
    QUEST_D->update_objective(killer, "border_gargoyle_raider", "raider_eliminated", 1);
    QUEST_D->complete_quest(killer, "border_gargoyle_raider");
    tell_object(killer,
        "The raider falls short of the wire. No man's land is quiet "
        "again, for now.\n");
}
