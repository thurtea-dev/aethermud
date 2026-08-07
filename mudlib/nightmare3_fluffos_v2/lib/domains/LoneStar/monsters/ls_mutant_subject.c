// /domains/LoneStar/monsters/ls_mutant_subject.c
// Escaped Lone Star genetic experiment.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("mutant subject");
    set_npc_name("the mutant");
    set_id(({ "mutant subject", "mutant", "subject", "experiment",
              "specimen", "escapee" }));
    set_short("a twisted mutant subject");
    set_long(
        "Something that was once humanoid. The Coalition gene labs reshaped\n"
        "the body: elongated limbs, patchwork skin, and eyes that reflect\n"
        "light like an animal's. Restraint scars ring the wrists. It moves\n"
        "with jerky, unpredictable speed.");
    set_level(6);
    set_max_hp(70);
    set_hp(70);
    set_aggressive(1);
    set_moving(0);
    set_race("mutant");
    set_alignment(-30);
    set_body_type("humanoid");
    set_property("mdc_creature", 1);
    set_stats("MDC", 20);
    set_stats("max_MDC", 20);
    set_stats("SDC", 15);
    set_stats("max_SDC", 15);
    set_property("no backstab", 0);

    add_response("help",
        "The mutant subject hisses and backs away, claws raised.");
    add_response("coalition",
        "The mutant flinches at the word and snarls.");
    set_default_response(
        "The mutant subject watches you with feral, unfocused eyes.");
}
