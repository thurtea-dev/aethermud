// /domains/PuertoAngel/npcs/pa_dockmaster.c
// Vampire dockmaster of Puerto Angel harbor.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("dockmaster");
    set_npc_name("the dockmaster");
    set_id(({ "dockmaster", "vampire", "secondary vampire", "harbor master",
              "old vampire", "patron" }));
    set_short("the harbor dockmaster");
    set_long(
        "An old secondary vampire in a weathered captain's coat. Decades of\n"
        "sun-bleached harbor work show in the lines of a face that no longer\n"
        "needs sleep. The dockmaster runs Puerto Angel's port with quiet\n"
        "efficiency and expects the same from visitors.");
    set_level(8);
    set_max_hp(120);
    set_hp(120);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("secondary vampire");
    set_alignment(-150);
    set_body_type("humanoid");

    set_stats("MDC", 90);
    set_stats("max_MDC", 90);
    set_stats("rifts_hp", 70);
    set_stats("max_rifts_hp", 70);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("is_MDC", 1);

    set_property("vampire", 1);
    set_property("mdc_creature", 1);
    set_property("undead", 1);
    set_property("visible_race", "human");
    setenv("rifts_race", "secondary vampire");

    add_response("hello", "The dockmaster nods. 'State your business on my docks.'");
    add_response("harbor", "The dockmaster says: 'Boats come and go. Cargo moves. "
        "Ask no questions you do not want answered.'");
    add_response("trade", "The dockmaster says: 'Fish, salvage, and discretion. "
        "Pay your fees and keep the peace.'");
    add_response("vampire", "The dockmaster's eyes narrow. 'Yes. And?'");
    add_response("bye", "The dockmaster turns back to the ledger on his crate.");
    set_default_response(
        "The dockmaster watches the harbor without looking directly at you.");
}
