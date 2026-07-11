/* /domains/Horton/monsters/horton_scavenger.c
   Desperate scavenger at the abandoned farmstead. Level 5, SDC 55. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    object kit;

    ::create();

    set_name("Horton scavenger");
    set_id( ({ "horton scavenger", "scavenger", "desperate scavenger",
               "raider", "human" }) );
    set_short("a desperate scavenger");
    set_long(
        "A gaunt human in filthy salvaged clothing, clutching a battered\n"
        "pistol and a knife taped to one forearm. Eyes dart between you and\n"
        "the farmhouse foundation like something valuable might still be\n"
        "buried there. Desperation makes people dangerous.\n"
    );

    set_race("human");
    set_class("scavenger");
    set_level(5);
    set_alignment(-200);
    set_aggressive(1);
    set_gender("neutral");
    set_body_type("humanoid");

    set_stats("IQ", 9);
    set_stats("ME", 9);
    set_stats("MA", 6);
    set_stats("PS", 13);
    set_stats("PP", 12);
    set_stats("PE", 13);
    set_stats("PB", 5);
    set_stats("Spd", 15);

    set_max_hp(35);
    set_hp(35);
    set_stats("SDC", 55);
    set_stats("max_SDC", 55);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 35);
    set_stats("max_rifts_hp", 35);

    kit = clone_object("/domains/Horton/equipment/basic_medkit");
    if(kit) kit->move(this_object());
}
