// /domains/Praxis/monsters/kittani_overseer.c
// Kittani dock overseer at Splynn.

#include <std.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("kittani overseer");
    set_id(({ "overseer", "kittani", "kittani overseer", "boss" }));
    set_short("a Kittani dock overseer");
    set_long(
        "A tall alien in segmented armor with a shock prod and datapad.\n"
        "Four arms move with mechanical efficiency. Slaves fear this one.");
    set_race("kittani");
    set_level(6);
    set_max_hp(100);
    set_hp(100);
    set_alignment(-300);
    set_aggressive(0);
    set_moving(0);
    set_gender("neuter");
    set_stats("MDC", 60);
    set_stats("max_MDC", 60);
    add_response("hello", "The overseer clicks: 'Work or be recycled.'");
    set_default_response("The Kittani overseer watches the dock with cold efficiency.");
}
