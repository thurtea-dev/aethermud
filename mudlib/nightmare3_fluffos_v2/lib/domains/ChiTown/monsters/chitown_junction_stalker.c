/* /domains/ChiTown/monsters/chitown_junction_stalker.c
   Whatever nested in the collapsed section of the maintenance tunnels.
   Level 7, SDC 90. Pays off chitown_maintenance_tunnel.c's "something
   has nested in the east junction" line. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("junction stalker");
    set_id( ({ "junction stalker", "stalker", "beast", "animal" }) );
    set_short("a junction stalker");
    set_long(
        "Something big-boned and patchy-hided has made a nest in the\n"
        "collapsed void above the junction, dropping down through the\n"
        "cracked ceiling to feed. Its forelimbs end in claws too long and\n"
        "too many-jointed for anything that should be living under a city.\n");

    set_race("animal");
    set_class("beast");
    set_level(7);
    set_alignment(-250);
    set_aggressive(1);
    set_gender("neutral");
    set_body_type("animal");

    set_stats("IQ", 6);
    set_stats("ME", 8);
    set_stats("MA", 4);
    set_stats("PS", 17);
    set_stats("PP", 16);
    set_stats("PE", 15);
    set_stats("PB", 2);
    set_stats("Spd", 18);

    set_max_hp(55);
    set_hp(55);
    set_stats("SDC", 90);
    set_stats("max_SDC", 90);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 55);
    set_stats("max_rifts_hp", 55);
}

int query_attack_damage() {
    return random(7) + 5;
}
