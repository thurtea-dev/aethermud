// /domains/demon_plane/monsters/demon_sliver_alpha.c
// Shrine guardian. Heavy MDC demon.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("demon sliver alpha");
    set_id(({ "demon sliver alpha", "alpha", "demon", "guardian" }));
    set_short("a demon sliver alpha");
    set_long(
        "This demon is twice the size of the hunters you have seen. MDC plates\n"
        "fuse into a horned carapace. Green fire leaks from joints when it\n"
        "breathes. It stands between you and the altar with obvious intent.");

    set_race("demon");
    set_class("predator");
    set_level(9);
    set_exp(55000);
    set_alignment(-1000);
    set_aggressive(1);

    set_stats("IQ", 10);
    set_stats("ME", 12);
    set_stats("MA", 8);
    set_stats("PS", 18);
    set_stats("PP", 16);
    set_stats("PE", 16);
    set_stats("PB", 4);
    set_stats("Spd", 14);

    set_stats("rifts_hp", 80);
    set_stats("max_rifts_hp", 80);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("MDC", 45);
    set_stats("max_MDC", 45);
    set_stats("is_MDC", 1);

    set_property("mdc_creature", 1);
    set_property("demon_plane", 1);
    setenv("rifts_flags", "mdc");

    set_hp(180);
    set_mp(40);
    set_sp(180);
}
