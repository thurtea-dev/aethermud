/* /domains/ChiTown/monsters/chitown_tunnel_rat.c
   Aggressive SDC mutant rat in the Chi-Town maintenance tunnels.
   Level 3, SDC 35. Flavor only, no special abilities this pass. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("tunnel rat");
    set_id( ({ "tunnel rat", "rat", "large tunnel rat", "animal" }) );
    set_short("a large tunnel rat");
    set_long(
        "An oversized rat, easily the size of a dog, with patchy fur and\n"
        "eyes that catch what little light reaches this far down. The\n"
        "maintenance crews stopped coming down here for a reason.\n"
    );

    set_race("animal");
    set_class("beast");
    set_level(3);
    set_exp(8000);
    set_alignment(-50);
    set_aggressive(1);
    set_gender("neutral");
    set_body_type("animal");

    set_stats("IQ", 3);
    set_stats("ME", 6);
    set_stats("MA", 3);
    set_stats("PS", 11);
    set_stats("PP", 13);
    set_stats("PE", 12);
    set_stats("PB", 2);
    set_stats("Spd", 16);

    set_max_hp(20);
    set_hp(20);
    set_stats("SDC", 35);
    set_stats("max_SDC", 35);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 20);
    set_stats("max_rifts_hp", 20);
}
