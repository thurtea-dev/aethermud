/* /domains/Lazlo/monsters/lazlo_feral_dog.c
   Feral scavenger dog on the Lazlo lake shore. SDC creature, level 3.
   Soloable by levels 2-4. No MDC. Standard bite attack. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("feral dog");
    set_id( ({ "feral dog", "dog", "mutt", "animal", "scavenger" }) );
    set_short("a feral dog");
    set_long(
        "A big, rough-coated mutt working the shoreline for scraps. Ribs\n"
        "visible through the coat, but the muscles underneath are solid.\n"
        "Eyes flat and cautious. It has been surviving out here for a while\n"
        "and does not intend to share this patch of shore with anyone.");

    set_race("dog");
    set_class("animal");
    set_level(3);
    set_alignment(-50);
    set_aggressive(1);
    set_body_type("animal");
    set_gender("neutral");

    set_stats("IQ", 4);
    set_stats("ME", 8);
    set_stats("MA", 4);
    set_stats("PS", 9);
    set_stats("PP", 14);
    set_stats("PE", 13);
    set_stats("PB", 3);
    set_stats("Spd", 22);

    set_stats("SDC", 25);
    set_stats("max_SDC", 25);

    set_hp(18);
    set_mp(0);
    set_sp(60);
}

int query_attack_damage() {
    return random(4) + 2;
}
