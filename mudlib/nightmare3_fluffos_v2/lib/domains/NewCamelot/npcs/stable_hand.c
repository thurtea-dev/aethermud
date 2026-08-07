/* /domains/NewCamelot/npcs/stable_hand.c
   a royal stable hand. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("stable hand");
    set_npc_name("Stable Hand");
    set_id( ({ "stable hand", "stablehand", "hand", "groom" }) );
    set_short("a royal stable hand");
    set_long(
        "A broad-shouldered youth with straw in his hair and a healthy\n"
        "respect for everything in the stalls. He carries a feed bucket in\n"
        "one hand and a stun prod in the other, and can tell you exactly\n"
        "which of the two he uses more.");
    set_level(3);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(200);
    set_body_type("human");
    add_response("hello",
        "Stable Hand says: 'Mind the grey mare. She bites. And the thing next to her spits acid.'");
    add_response("horses",
        "Stable Hand says: 'Knights' mounts. Some from this world, some not.'");
    add_response("bye",
        "Stable Hand says: 'Watch your step on the way out. Seriously.'");

    set_default_response("The stable hand hauls feed down the aisle.");
}
