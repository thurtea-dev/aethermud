/* /domains/PuertoAngel/npcs/pa_barkeep.c
   Puerto Angel cantina barkeep -- ancient secondary vampire, civil and tolerant. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("barkeep");
    set_npc_name("barkeep");
    set_id( ({ "barkeep", "bartender", "vampire barkeep", "ancient", "old one",
               "keeper", "pa barkeep" }) );
    set_short("the vampire barkeep");
    set_long(
        "Ancient. The word fits precisely. They were old before the Rifts, and\n"
        "the Rifts simply made them more so. The skin is translucent, the eyes\n"
        "a deep, still amber. They move with the patience of something that has\n"
        "learned there is no point in hurry. They pour a drink without looking,\n"
        "without spilling. They are civil, which in Puerto Angel passes for kind.");
    set_level(12);
    set_max_hp(200);
    set_hp(200);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("secondary vampire");
    set_alignment(-100);
    set_body_type("humanoid");

    /* MDC-based being */
    set_stats("MDC", 120);
    set_stats("max_MDC", 120);
    set_stats("is_MDC", 1);
    set_property("vampire", 1);
    set_property("mdc_creature", 1);
    set_property("undead", 1);

    add_response("hello",
        "The barkeep says: 'Welcome to Puerto Angel. We ask only that you cause no trouble. "
        "We have enough of our own.'");
    add_response("hi",
        "The barkeep says: 'Welcome. Sit, if you wish. What can I offer you?'");
    add_response("information",
        "The barkeep says: 'Cross water? Vampires cannot. Silver hurts us. Fire hurts us more. "
        "We know what we are. We simply prefer it this way.'");
    add_response("weakness",
        "The barkeep says: 'I will tell you freely. Silver. Running water. Sunlight, "
        "eventually. Holy symbols if wielded with genuine faith. We are difficult to kill "
        "but not impossible. We survive by not inviting conflict.'");
    add_response("vampire",
        "The barkeep says: 'We are what we are. This town was dying before the Rifts. "
        "Now it is not dying. We consider that an improvement.'");
    add_response("town",
        "The barkeep says: 'Puerto Angel is ours. We maintain it, we defend it, we ask "
        "visitors to conduct themselves with courtesy. Those who do are welcome to stay.'");
    add_response("danger",
        "The barkeep says: 'Harm no one here and no one will harm you. "
        "Cause trouble and the town will become very quiet in ways you will not like.'");
    add_response("blood",
        "The barkeep says: 'We have arrangements. Willing donors, animals, "
        "what is available. The old way was wasteful. We have learned patience.'");
    add_response("drink",
        "The barkeep slides something dark across the bar. "
        "'Blood-wine. An acquired taste. No obligation to finish it.'");
    add_response("blood-wine",
        "The barkeep slides something dark across the bar. "
        "'House specialty. It is what it sounds like. You will either find it "
        "interesting or you will not.'");
    add_response("bye",
        "The barkeep says: 'Travel safely. The road north is long.'");
    set_default_response(
        "The barkeep listens with ancient patience and says: "
        "'That is an interesting question. I have been thinking about it for some time.'");
}

void init() {
    ::init();
}
