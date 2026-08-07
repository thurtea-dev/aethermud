/* /domains/NGR/npcs/ngr_gate_guard.c
   NGR gate guard -- neutral unless provoked.
   Heavy power armor, rail gun. Protects the NGR main gate. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("NGR gate guard");
    set_npc_name("NGR gate guard");
    set_id( ({ "ngr gate guard", "gate guard", "guard", "soldier", "ngr soldier",
               "ngr guard" }) );
    set_short("an NGR gate guard");
    set_long(
        "A soldier of the New German Republic in full grey combat power armor.\n"
        "The visor is a one-way mirror: you cannot see the face inside.\n"
        "A rail gun is held at port arms, finger indexed outside the trigger.\n"
        "The armor is immaculate: no scratches, no dust. This soldier is\n"
        "professional in a way that makes you instinctively careful about\n"
        "what you say and where you put your hands.");

    set_level(8);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(100);
    set_body_type("humanoid");

    set_stats("IQ", 13);
    set_stats("ME", 14);
    set_stats("MA", 11);
    set_stats("PS", 18);
    set_stats("PP", 16);
    set_stats("PE", 17);
    set_stats("PB", 10);
    set_stats("Spd", 20);

    set_stats("rifts_hp",     80);
    set_stats("max_rifts_hp", 80);
    set_stats("SDC",          50);
    set_stats("max_SDC",      50);
    set_stats("MDC",          120);
    set_stats("max_MDC",      120);

    set_exp(8000);
    set_property("mdc_creature", 0);
    set_property("coalition_type", "ngr");
    set_property("AR", 16);
    setenv("rifts_race", "human");
    setenv("rifts_occ",  "ngr soldier");

    add_response("hello",
        "The guard gives a single nod. 'Identification and purpose of visit.'");
    add_response("hi",
        "NGR gate guard says, flatly: 'State your business.'");
    add_response("identification",
        "NGR gate guard says: 'Documents are processed at the checkpoint station. "
        "All visitors must register. Non-compliance is not an option here.'");
    add_response("papers",
        "NGR gate guard says: 'Documents. Checkpoint. Over there. Move.'");
    add_response("ngr",
        "NGR gate guard says: 'New German Republic. You are inside its borders. "
        "Conduct yourself accordingly.'");
    add_response("triax",
        "NGR gate guard says: 'Triax is the NGR's manufacturing partner. "
        "You will find their equipment in the market district.'");
    add_response("danger",
        "NGR gate guard says: 'The NGR manages threats within its borders. "
        "You will not find Brodkil, Gargoyles, or Splugorth here.'");
    add_response("market",
        "NGR gate guard says: 'East gate. You are already past it.'");
    add_response("bye",
        "The guard watches you go. Probably.");
    set_default_response(
        "The guard stares at you through the mirrored visor. "
        "'Move along or state your business.'");
}

void init() {
    ::init();
}
