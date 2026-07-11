/* /domains/wizards/mdc_training_dummy.c
   MDC combat robot training target for the combat testing zone.
   Passive -- does not fight back. Respawns via room reset.
   Requires MDC weapons or psi-sword to damage meaningfully. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("combat robot");
    set_id( ({ "combat robot", "robot", "mdc robot", "mdc dummy", "target" }) );
    set_short("an MDC combat robot");
    set_long(
        "A decommissioned Coalition patrol bot repurposed as a test target.\n"
        "Its MDC plating is scarred from previous sessions. Standard SDC\n"
        "weapons bounce harmlessly off its hull. Only MDC weapons, psi-swords,\n"
        "or equivalent MDC attacks will register damage. It does not fight back.\n"
    );
    set_race("human");
    set_level(1);
    set_alignment(0);
    set_max_hp(50);
    set_hp(50);
    set_stats("strength", 12);
    set_stats("dexterity", 1);
    set_stats("constitution", 10);
    set_stats("rifts_hp", 50);
    set_property("position_str", "stands at the second target station, plating scarred from testing.");
    set_property("aggressive", 0);
    set_property("passive", 1);
    set_property("no_xp", 1);
    set_property("no_loot", 1);
    set_property("mdc_creature", 1);
    set_stats("MDC", 300);
    set_stats("max_MDC", 300);
}
