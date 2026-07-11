// /domains/demon_plane/monsters/demon_sliver_stalker.c
// Lesser MDC demon-plane predator.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("demon sliver stalker");
    set_id(({ "demon sliver stalker", "stalker", "demon", "predator" }));
    set_short("a demon sliver stalker");
    set_long(
        "A low-slung predator built from chitin and shadow. Four segmented legs\n"
        "carry a body wrapped in MDC plates that ripple when it moves. A lamprey\n"
        "mouth glistens with black ichor. It hunts by vibration.");

    set_race("demon");
    set_class("predator");
    set_level(5);
    set_exp(18000);
    set_alignment(-800);
    set_aggressive(1);

    set_stats("IQ", 6);
    set_stats("ME", 8);
    set_stats("MA", 6);
    set_stats("PS", 14);
    set_stats("PP", 14);
    set_stats("PE", 12);
    set_stats("PB", 4);
    set_stats("Spd", 16);

    set_stats("rifts_hp", 40);
    set_stats("max_rifts_hp", 40);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("MDC", 18);
    set_stats("max_MDC", 18);
    set_stats("is_MDC", 1);

    set_property("mdc_creature", 1);
    set_property("demon_plane", 1);
    setenv("rifts_flags", "mdc");

    set_hp(80);
    set_mp(20);
    set_sp(100);
}

void die() {
    object env;

    env = environment(this_object());
    if(env && random(100) < 35) {
        object ichor;
        ichor = clone_object("/domains/demon_plane/equipment/demon_ichor_vial");
        if(ichor) ichor->move(env);
    }
    ::die();
}
