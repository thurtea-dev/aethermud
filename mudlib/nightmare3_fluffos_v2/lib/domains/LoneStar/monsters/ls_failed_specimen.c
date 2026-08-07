/* /domains/LoneStar/monsters/ls_failed_specimen.c
   Lone Star cold storage threat: a surplus line that woke up in the cradle.
   Tougher than ls_mutant_subject; the cold storage room's reason to exist. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("failed specimen");
    set_npc_name("the specimen");
    set_id( ({ "failed specimen", "specimen", "surplus", "failure",
               "thing", "escapee" }) );
    set_short("a failed specimen, thawed and moving");
    set_long(
        "A Dog Boy line taken far enough off spec that the Coalition stamped\n"
        "it SURPLUS and put it on a rack to die quietly. It did not. Frost\n"
        "still rimes the joints and the torn sleeve hangs from one shoulder\n"
        "like shed skin. The build is wrong in every direction at once: too\n"
        "long through the arms, too heavy through the jaw, and the eyes have\n"
        "no reflective layer left in them at all.\n"
        "It does not appear to feel the cold, or much else.");
    set_level(9);
    set_max_hp(120);
    set_hp(120);
    set_aggressive(1);
    set_moving(0);
    set_gender("neutral");
    set_race("mutant");
    set_alignment(-60);
    set_body_type("humanoid");
    set_property("mdc_creature", 1);
    set_stats("MDC", 45);
    set_stats("max_MDC", 45);
    set_stats("SDC", 25);
    set_stats("max_SDC", 25);
    set_property("no backstab", 0);

    add_response("coalition",
        "The specimen's jaw works soundlessly around the word.");
    add_response("surplus",
        "The specimen turns its head toward the stamped tag on its own "
        "torn sleeve, and then toward you.");
    set_default_response(
        "The specimen tracks you without blinking. Nothing behind the eyes "
        "answers.");

    set_die( (: "on_death" :) );
}

void init() {
    ::init();
}

void on_death(object killer) {
    if(!killer || !objectp(killer)) killer = this_player();
    if(!killer || !objectp(killer)) return;
    if(!(int)QUEST_D->is_quest_active(killer, "cold_storage_escapee")) return;
    QUEST_D->update_objective(killer, "cold_storage_escapee", "specimen_destroyed", 1);
    QUEST_D->complete_quest(killer, "cold_storage_escapee");
    tell_object(killer,
        "The surplus line stops moving. Whatever the kennel master owed "
        "you, you have earned it.\n");
}
