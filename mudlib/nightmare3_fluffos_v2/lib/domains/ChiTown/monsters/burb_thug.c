/* /domains/ChiTown/monsters/burb_thug.c
   Aggressive SDC thug in gang territory. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("burb thug");
    set_id( ({ "burb thug", "thug", "ganger", "human" }) );
    set_short("a burb thug");
    set_long(
        "A scarred human in scavenged armor plates, chain wrapped around\n"
        "one fist. The gang corner is claimed ground, and this one is here\n"
        "to collect respect or blood.");

    set_race("human");
    set_class("fighter");
    set_level(5);
    set_exp(15000);
    set_alignment(-300);
    set_aggressive(1);
    set_gender("male");
    set_body_type("human");

    set_stats("IQ", 8);
    set_stats("ME", 8);
    set_stats("MA", 6);
    set_stats("PS", 15);
    set_stats("PP", 12);
    set_stats("PE", 14);
    set_stats("PB", 6);
    set_stats("Spd", 14);

    set_max_hp(35);
    set_hp(35);
    set_stats("SDC", 60);
    set_stats("max_SDC", 60);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 35);
    set_stats("max_rifts_hp", 35);
}
