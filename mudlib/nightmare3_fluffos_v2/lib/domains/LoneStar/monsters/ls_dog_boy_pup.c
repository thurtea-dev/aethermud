/* /domains/LoneStar/monsters/ls_dog_boy_pup.c
   Juvenile Dog Boy in the Lone Star kennel wing, not yet assigned a handler.
   Low level, not aggressive. Attacking one is a choice, not an ambush. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("dog boy pup");
    set_npc_name("the pup");
    set_id( ({ "dog boy pup", "pup", "juvenile", "young dog boy",
               "trainee" }) );
    set_short("a juvenile Dog Boy in a numbered collar");
    set_long(
        "Barely waist high, canine-featured, and moving in the fast nervous\n"
        "bursts of something that has never been outside. A numbered collar\n"
        "sits loose on the neck; the number is stenciled, not engraved,\n"
        "because it is expected to change. It watches your hands rather than\n"
        "your face, and flinches at the sound of the whistle.");
    set_level(3);
    set_max_hp(35);
    set_hp(35);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("dog boy");
    set_alignment(-10);
    set_body_type("humanoid");
    set_property("mdc_creature", 1);
    set_stats("MDC", 10);
    set_stats("max_MDC", 10);
    set_stats("SDC", 12);
    set_stats("max_SDC", 12);

    add_response("hello",
        "The pup's ears go flat. It does not answer. It may not have been "
        "taught how.");
    add_response("magic",
        "The pup's head comes up hard and it fixes on you, nose working. "
        "That part of the conditioning has taken.");
    add_response("name",
        "The pup touches the stenciled number on its own collar and looks "
        "away.");
    set_default_response(
        "The pup watches your hands and stays just out of reach.");
}

void init() {
    ::init();
}
