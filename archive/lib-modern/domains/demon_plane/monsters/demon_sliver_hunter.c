// /domains/demon_plane/monsters/demon_sliver_hunter.c
// Mid-tier MDC demon-plane hunter.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("demon sliver hunter");
    set_id(({ "demon sliver hunter", "hunter", "demon", "pack hunter" }));
    set_short("a demon sliver hunter");
    set_long(
        "A bipedal demon armored in overlapping MDC scales. Spines run down\n"
        "its back and forearms end in scythe claws. It moves with pack discipline,\n"
        "cutting off exits before it strikes.");

    set_race("demon");
    set_class("predator");
    set_level(7);
    set_exp(32000);
    set_alignment(-900);
    set_aggressive(1);

    set_stats("IQ", 8);
    set_stats("ME", 10);
    set_stats("MA", 7);
    set_stats("PS", 16);
    set_stats("PP", 15);
    set_stats("PE", 14);
    set_stats("PB", 5);
    set_stats("Spd", 15);

    set_stats("rifts_hp", 55);
    set_stats("max_rifts_hp", 55);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("MDC", 30);
    set_stats("max_MDC", 30);
    set_stats("is_MDC", 1);

    set_property("mdc_creature", 1);
    set_property("demon_plane", 1);
    setenv("rifts_flags", "mdc");

    set_hp(120);
    set_mp(30);
    set_sp(140);
}
