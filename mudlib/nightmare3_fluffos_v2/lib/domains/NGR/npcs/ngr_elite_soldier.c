/* /domains/NGR/npcs/ngr_elite_soldier.c
   NGR elite soldier in Triax armor. Aggressive. Level 10, MDC 200. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("NGR elite soldier");
    set_npc_name("NGR elite soldier");
    set_id( ({ "ngr elite soldier", "elite soldier", "soldier",
               "ngr soldier", "elite" }) );
    set_short("an NGR elite soldier in Triax armor");
    set_long(
        "A veteran of the NGR Special Operations Group in full Triax PA-09\n"
        "power armor. The suit is larger than standard infantry issue: broader\n"
        "shoulders, heavier plating on the torso, and the servo-hum of actuators\n"
        "with more power behind them. The visor is black. The weapon is a\n"
        "Triax rail gun capable of putting holes in vehicles. This soldier\n"
        "is stationed here for a reason, and the reason is you.\n"
    );

    set_level(10);
    set_aggressive(1);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(100);
    set_body_type("humanoid");

    set_stats("IQ", 14);
    set_stats("ME", 15);
    set_stats("MA", 12);
    set_stats("PS", 24);
    set_stats("PP", 18);
    set_stats("PE", 20);
    set_stats("PB", 10);
    set_stats("Spd", 22);

    set_stats("rifts_hp",     200);
    set_stats("max_rifts_hp", 200);
    set_stats("MDC",          200);
    set_stats("max_MDC",      200);

    set_exp(60000);
    set_property("mdc_creature", 1);
    set_property("coalition_type", "ngr");
    set_property("AR", 17);
    setenv("rifts_race", "human");
    setenv("rifts_occ",  "ngr soldier");
}

void init() {
    ::init();
}
