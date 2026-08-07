/* /domains/NGR/npcs/ngr_patrol_soldier.c
   NGR patrol soldier on road duty. Aggressive. Level 7, MDC 130. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("NGR patrol soldier");
    set_npc_name("NGR patrol soldier");
    set_id( ({ "ngr patrol soldier", "patrol soldier", "soldier",
               "ngr soldier", "patrol" }) );
    set_short("an NGR patrol soldier");
    set_long(
        "An NGR soldier on road patrol in full combat armor, rail rifle held\n"
        "at low ready. The armor is grey with road dust. This one watches\n"
        "the treeline and the road with equal attention. Patrols here have\n"
        "contact with Gargoyle raiders at irregular intervals and have learned\n"
        "to treat every approach as potentially hostile.\n"
    );

    set_level(7);
    set_aggressive(1);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(100);
    set_body_type("humanoid");

    set_stats("IQ", 13);
    set_stats("ME", 13);
    set_stats("MA", 11);
    set_stats("PS", 18);
    set_stats("PP", 16);
    set_stats("PE", 17);
    set_stats("PB", 10);
    set_stats("Spd", 20);

    set_stats("rifts_hp",     130);
    set_stats("max_rifts_hp", 130);
    set_stats("MDC",          130);
    set_stats("max_MDC",      130);

    set_exp(20000);
    set_property("mdc_creature", 1);
    set_property("coalition_type", "ngr");
    set_property("AR", 15);
    setenv("rifts_race", "human");
    setenv("rifts_occ",  "ngr soldier");
}

void init() {
    ::init();
}
