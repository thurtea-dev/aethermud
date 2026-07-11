// /d/Praxis/monsters/wild_vampire.c
// Wild vampire -- lair dweller in the collapsed office building.
// MDC being; aggressive. Attacks with bite (1d6 HP drain) and claw (2d6 SDC).
// Holy items deal double damage (checked via property "holy" on weapon).

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("wild vampire");
    set_id( ({ "wild vampire", "vampire", "undead", "creature" }) );
    set_short("a wild vampire");
    set_long(
        "It was human once. Now the proportions are wrong: too tall, limbs\n"
        "too long, fingers ending in cracked black nails. Its eyes are a flat\n"
        "reflective silver in the dark. It moves without sound and has been\n"
        "watching you since you entered. Wild vampires operate on instinct:\n"
        "hunt, feed, shelter. This one has found all three here.");

    set_race("vampire");
    set_class("undead");
    set_level(5);
    set_alignment(-800);
    set_aggressive(1);
    set_body_type("humanoid");

    set_stats("IQ", 8);
    set_stats("ME", 10);
    set_stats("MA", 6);
    set_stats("PS", 20);
    set_stats("PP", 17);
    set_stats("PE", 18);
    set_stats("PB", 6);
    set_stats("Spd", 20);

    // MDC being with no SDC pool
    set_stats("MDC", 80);
    set_stats("max_MDC", 80);
    set_stats("rifts_hp", 60);
    set_stats("max_rifts_hp", 60);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("is_MDC", 1);

    setenv("rifts_race", "vampire");
    setenv("rifts_flags", "undead");

    set_hp(120);
    set_mp(20);
    set_sp(100);

    set_property("undead", 1);
    set_property("no_corpse", 1);
    set_property("visible_race", "human");
    set_die((:"die_vampire":));
}

// Override apply_hit to add vampire combat flavour and holy weakness.
// The actual damage math is handled by rifts_combat.c; this is for messaging.
void heart_beat() {
    object attacker;

    ::heart_beat();
    attacker = query_current_attacker();
    if(attacker && !random(4)) {
        tell_room(environment(this_object()),
            "The wild vampire hisses and slashes with elongated claws.\n",
            ({ this_object() }));
    }
}

int die_vampire(object killer) {
    tell_room(environment(this_object()),
        "The wild vampire collapses, its form darkening and desiccating with "
        "unnatural speed. Within moments nothing remains but ash and silence.\n",
        ({ this_object() }));
    if(killer && objectp(killer)) killer->add_exp(800);
    if(this_object()) destruct(this_object());
    return 0; // stops normal death processing (no corpse)
}
