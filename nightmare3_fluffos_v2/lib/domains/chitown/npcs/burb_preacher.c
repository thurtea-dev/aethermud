// /domains/chitown/npcs/burb_preacher.c
// Street preacher in the burbs square.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("street preacher");
    set_npc_name("Preacher");
    set_id( ({ "street preacher", "preacher", "human", "man" }) );
    set_short("a street preacher");
    set_long(
        "A thin human in a patched coat, standing on a crate with a worn\n"
        "book in one hand. Voice hoarse from shouting over the burbs noise.\n"
        "Eyes bright with conviction or desperation - hard to tell which.");
    set_level(3);
    set_max_hp(35);
    set_hp(35);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(150);
    set_body_type("human");

    set_stats("SDC", 30);
    set_stats("max_SDC", 30);

    add_response("hello",
        "Preacher calls: 'The Rifts are judgment and mercy both! Hear me!'");
    add_response("hi",
        "Preacher calls: 'The Rifts are judgment and mercy both! Hear me!'");
    add_response("rifts",
        "Preacher: 'Gateways of the old sins. Walk carefully, child.'");
    add_response("coalition",
        "Preacher lowers his voice: 'They build walls against monsters and "
        "against hope. Do not confuse the two.'");
    add_response("magic",
        "Preacher glances toward the wall: 'Not a word for this street. "
        "Not if you want to keep breathing.'");
    set_default_response(
        "The preacher returns to shouting at the crowd.");
}
