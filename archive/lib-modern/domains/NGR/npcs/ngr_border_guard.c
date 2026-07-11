/* /domains/NGR/npcs/ngr_border_guard.c
   NGR border guard at the checkpoint. Non-aggressive. Level 5, MDC 100. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("NGR border guard");
    set_npc_name("NGR border guard");
    set_id( ({ "ngr border guard", "border guard", "guard", "soldier",
               "ngr soldier", "ngr guard" }) );
    set_short("an NGR border guard");
    set_long(
        "A checkpoint soldier of the New German Republic. Grey body armor,\n"
        "sealed visor, rail pistol holstered. This one processes travelers\n"
        "rather than engaging them: a clipboard terminal, an eye scanner,\n"
        "and the practiced patience of someone who has reviewed ten thousand\n"
        "sets of travel documents and found problems in roughly half of them.\n"
    );

    set_level(5);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(100);
    set_body_type("humanoid");

    set_stats("IQ", 12);
    set_stats("ME", 12);
    set_stats("MA", 10);
    set_stats("PS", 16);
    set_stats("PP", 14);
    set_stats("PE", 15);
    set_stats("PB", 10);
    set_stats("Spd", 18);

    set_stats("rifts_hp",     100);
    set_stats("max_rifts_hp", 100);
    set_stats("MDC",          100);
    set_stats("max_MDC",      100);

    set_exp(5000);
    set_property("mdc_creature", 1);
    set_property("coalition_type", "ngr");
    set_property("AR", 14);
    setenv("rifts_race", "human");
    setenv("rifts_occ",  "ngr soldier");

    add_response("hello",
        "The guard says: 'Identification and purpose of travel. Documents out.'");
    add_response("hi",
        "The guard says, without looking up: 'Documents.'");
    add_response("identification",
        "The guard says: 'Scan the terminal to your left. All visitors. No exceptions.'");
    add_response("documents",
        "The guard says: 'Terminal. Left. Now.'");
    add_response("checkpoint",
        "The guard says: 'Standard border processing. Five minutes if you cooperate.'");
    add_response("ngr",
        "The guard says: 'You are in NGR territory. Conduct yourself accordingly.'");
    add_response("triax",
        "The guard says: 'Triax depot is inside, east of the market. You will\n"
        "need a purchase permit from this station.'");
    add_response("bye",
        "The guard makes a note on the terminal and returns to processing.");
    set_default_response(
        "The guard says: 'State your purpose or move to the side. Others are waiting.'");
}

void init() {
    ::init();
}
