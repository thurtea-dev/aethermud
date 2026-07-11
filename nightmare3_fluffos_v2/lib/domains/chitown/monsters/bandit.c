// /domains/chitown/monsters/bandit.c
// Human scavenger bandit near Chi-Town. SDC-based combatant.
// Counts toward stolen_goods quest objective.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

private object __last_killer;

void cease_all_attacks() {
    object *att;

    att = query_attackers();
    if(att && sizeof(att) && att[0] && objectp(att[0]))
        __last_killer = att[0];
    ::cease_all_attacks();
}

void create() {
    ::create();

    set_name("bandit");
    set_id( ({ "bandit", "scavenger", "human scavenger", "raider" }) );
    set_short("a road bandit");
    set_long(
        "A grubby human in mismatched salvaged clothing, armed with a battered\n"
        "rifle. Desperate eyes. Pre-Rifts highway scavenger turned raider.\n"
        "The Coalition would shoot them on sight. You might not need to wait.\n"
    );

    set_race("human");
    set_class("scavenger");
    set_level(3);
    set_exp(10000);
    set_alignment(-300);
    set_aggressive(1);

    set_stats("IQ", 9);
    set_stats("ME", 8);
    set_stats("MA", 6);
    set_stats("PS", 13);
    set_stats("PP", 11);
    set_stats("PE", 12);
    set_stats("PB", 6);
    set_stats("Spd", 14);

    set_max_hp(40);
    set_hp(40);
    set_stats("SDC", 60);
    set_stats("max_SDC", 60);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 40);
    set_stats("max_rifts_hp", 40);

    set_die( (: "on_death" :) );
}

void on_death(object killer) {
    object salvage;

    if(!killer || !objectp(killer)) {
        if(__last_killer && objectp(__last_killer))
            killer = __last_killer;
        else
            killer = this_player();
    }
    if(killer && objectp(killer)) {
        if((int)QUEST_D->is_quest_active(killer, "stolen_goods")) {
            mapping aq;
            mapping prog;
            int count;
            aq = (mapping)QUEST_D->query_active_quests(killer);
            prog = (mapping)aq["stolen_goods"];
            count = prog ? (int)prog["bandits_killed"] : 0;
            QUEST_D->update_objective(killer, "stolen_goods", "bandits_killed", count + 1);
            if(count + 1 >= 3)
                tell_object(killer, "[Quest] Stolen Goods: All bandits eliminated. Report back to the Quartermaster.\n");
            else
                tell_object(killer, "[Quest] Stolen Goods: Bandits eliminated: " + (count + 1) + "/3.\n");
        }
        killer->add_exp(200);
    }
    salvage = clone_object("/domains/Praxis/equipment/salvage_item");
    if(salvage) salvage->move(environment(this_object()));
}
