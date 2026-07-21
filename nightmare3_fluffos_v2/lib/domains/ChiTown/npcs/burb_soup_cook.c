// /domains/ChiTown/npcs/burb_soup_cook.c
// Friendly soup-kitchen cook in the burbs.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("soup cook");
    set_npc_name("Cook");
    set_id( ({ "soup cook", "cook", "human", "man" }) );
    set_short("a soup cook");
    set_long(
        "A heavyset human in a stained apron, stirring a battered pot over\n"
        "a drum fire. Grease and ash streak both forearms. The cook watches\n"
        "the line of hungry Burbies with tired patience.");
    set_level(2);
    set_max_hp(40);
    set_hp(40);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(200);
    set_body_type("human");

    set_stats("SDC", 35);
    set_stats("max_SDC", 35);

    add_response("hello",
        "Cook says: 'Bowl is free if you wait your turn. Credits buy seconds.'");
    add_response("hi",
        "Cook says: 'Bowl is free if you wait your turn. Credits buy seconds.'");
    add_response("food",
        "Cook nods at the pot: 'Whatever we scavenged. Do not ask what is in it.'");
    add_response("soup",
        "Cook nods at the pot: 'Whatever we scavenged. Do not ask what is in it.'");
    add_response("coalition",
        "Cook spatters the fire: 'They eat better inside the wall. We eat what is left.'");
    add_response("bye",
        "Cook waves a ladle. 'Stay warm.'");
    set_default_response(
        "The cook keeps stirring and does not look up.");
}
