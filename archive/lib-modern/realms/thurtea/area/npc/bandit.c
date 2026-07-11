/* /realms/thurtea/area/npc/bandit.c */
/* QCS NPC by thurtea */

/* QCS NPC template - placeholders replaced at creation time. */
#include <std.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("Bandit");
    set_npc_name("Bandit");
    set_id( ({ "bandit" }) );
    set_short("a bandit");
    set_long("A newly created NPC awaiting description.");
    set_level(1);
    set_max_hp(20);
    set_hp(20);
    set_stats("rifts_hp", 20);
    set_aggressive(0);
    set_moving(0);
    set_gender("neuter");
    set_race("human");
    setenv("rifts_occ", "mercenary");
    set_alignment(0);
    set_body_type("human");
    set_property("mdc_creature", 0);
    set_stats("MDC", 0);
    set_stats("max_MDC", 0);
    set_default_response("The creature watches silently.");
}
