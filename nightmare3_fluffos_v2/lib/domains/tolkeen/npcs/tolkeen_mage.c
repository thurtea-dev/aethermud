// /domains/tolkeen/npcs/tolkeen_mage.c

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Tolkeen mage");
    set_npc_name("Mage");
    set_id( ({ "tolkeen mage", "mage", "wizard", "scholar" }) );
    set_short("a Tolkeen mage");
    set_long(
        "A lean mage in travel robes marked with Tolkeen's lightning fist.\n"
        "PPE hangs in the air around them like heat shimmer.");
    set_level(7);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(250);
    set_body_type("human");

    set_stats("PPE", 120);
    set_stats("max_PPE", 120);

    add_response("hello",
        "Mage: 'The city stands on ley and stubbornness. Learn which you trust more.'");
    add_response("ley",
        "Mage: 'Lines cross under the crypt. Do not go alone.'");
    add_response("coalition",
        "Mage: 'They call us demons. We call them cowards with better factories.'");
    set_default_response("The mage returns to a floating notebook.");
}
