// /d/Praxis/monsters/catacomb_wight.c
// Undead entity entombed beneath Splynn. MDC supernatural being.
// Life drain attack: each successful hit reduces target ME by 1 permanently.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("catacomb wight");
    set_id( ({ "catacomb wight", "wight", "undead", "figure", "thing", "creature" }) );
    set_short("a catacomb wight");
    set_long(
        "Whatever this was in life has been compressed into something\n"
        "that functions on stored malice. The shape is roughly human:\n"
        "two legs, two arms, a head at the correct elevation. The details\n"
        "are wrong. The skin is drawn tight over geometry that does not\n"
        "match any skeleton you know. The eyes are grey points of light\n"
        "in recessed sockets that never move, just track.\n\n"
        "Cold radiates from it. Not the cold of temperature. The cold\n"
        "of something that feeds on warmth and will not stop until\n"
        "there is nothing left to take.");

    set_race("wight");
    set_class("undead");
    set_level(9);
    set_exp(45000);
    set_alignment(-900);
    set_aggressive(1);

    set_stats("IQ", 10);
    set_stats("ME", 14);
    set_stats("MA", 4);
    set_stats("PS", 20);
    set_stats("PP", 15);
    set_stats("PE", 22);
    set_stats("PB", 2);
    set_stats("Spd", 12);

    set_stats("rifts_hp", 50);
    set_stats("max_rifts_hp", 50);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("MDC", 80);
    set_stats("max_MDC", 80);
    set_stats("is_MDC", 1);

    setenv("rifts_flags", "mdc_being;undead;life_drain");

    set_money("credits", 0);

    set_hp(120);
    set_mp(60);
    set_sp(50);

    set_property("damage_num", 2);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 2);
    set_property("damage_string", "2d6+2 MDC (life drain touch)");
    set_property("mdc_weapon", 1);
    set_property("is_MDC", 1);
    set_property("no_steal", 1);
    set_property("life_drain", 1);
}

void death_message(object attacker) {
    string name;
    name = (string)attacker->query_cap_name();
    tell_room(environment(this_object()),
        "The wight collapses inward, the grey light in its eyes guttering out.\n"
        "The cold in the room lifts. " + name + " stands in sudden warmth\n"
        "and silence. The sarcophagus is just stone now.\n",
        ({ }) );
}
