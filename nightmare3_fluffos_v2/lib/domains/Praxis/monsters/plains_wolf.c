// /d/Praxis/monsters/plains_wolf.c
// Plains wolf -- post-Rifts mutated wolf, larger and more aggressive.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    int hp;

    ::create();

    hp = (random(6)+1 + random(6)+1 + random(6)+1) * 5;

    set_name("plains wolf");
    set_id( ({ "plains wolf", "wolf", "animal", "predator" }) );
    set_short("a plains wolf");
    set_long(
        "A wolf that has grown past the natural limit. Two hundred pounds of\n"
        "scar-tracked muscle, low-slung and fast. The Rifts twisted the bloodline\n"
        "somewhere; the eyes are wrong -- a steady amber glow that suggests\n"
        "intelligence it should not have. Its hackles rise as it registers\n"
        "your presence. Plains wolves are not afraid of people.");

    set_race("wolf");
    set_class("animal");
    set_level(3);
    set_exp(7500);
    set_alignment(-100);
    set_aggressive(1);

    set_stats("IQ", 4);
    set_stats("ME", 8);
    set_stats("MA", 5);
    set_stats("PS", 12);
    set_stats("PP", 16);
    set_stats("PE", 14);
    set_stats("PB", 5);
    set_stats("Spd", 22);

    set_stats("rifts_hp", hp);
    set_stats("max_rifts_hp", hp);
    set_stats("SDC", 30);
    set_stats("max_SDC", 30);
    set_stats("MDC", 0);
    set_stats("max_MDC", 0);
    set_stats("is_MDC", 0);

    setenv("rifts_occ", "animal");
    setenv("rifts_flags", "");

    set_hp(hp + 30);
    set_mp(0);
    set_sp(60);

    set_body_type("animal");
}
