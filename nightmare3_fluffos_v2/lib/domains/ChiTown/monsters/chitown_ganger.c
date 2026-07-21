/* /domains/ChiTown/monsters/chitown_ganger.c
   Aggressive SDC human ganger in the Chi-Town lower market. Level 4, SDC 50. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("Chi-Town ganger");
    set_id( ({ "chi-town ganger", "ganger", "gang member", "human" }) );
    set_short("a Chi-Town ganger");
    set_long(
        "A lean figure in scavenged street armor, a length of pipe in one\n"
        "hand and a chipped knife at the belt. The lower market is their\n"
        "territory and they do not appreciate strangers wandering through\n"
        "it without paying the toll.\n"
    );

    set_race("human");
    set_class("ganger");
    set_level(4);
    set_exp(12000);
    set_alignment(-250);
    set_aggressive(1);
    set_gender("neutral");
    set_body_type("humanoid");

    set_stats("IQ", 9);
    set_stats("ME", 9);
    set_stats("MA", 7);
    set_stats("PS", 14);
    set_stats("PP", 13);
    set_stats("PE", 13);
    set_stats("PB", 7);
    set_stats("Spd", 16);

    set_max_hp(30);
    set_hp(30);
    set_stats("SDC", 50);
    set_stats("max_SDC", 50);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 30);
    set_stats("max_rifts_hp", 30);
}
