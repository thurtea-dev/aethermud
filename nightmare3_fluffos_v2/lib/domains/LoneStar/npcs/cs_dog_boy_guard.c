// /domains/LoneStar/npcs/cs_dog_boy_guard.c
// Coalition Dog Boy guard stationed at Lone Star complex.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Dog Boy guard");
    set_npc_name("the Dog Boy");
    set_id( ({ "dog boy", "dog boy guard", "guard", "coalition guard",
               "cs guard", "dog", "boy" }) );
    set_short("a Coalition Dog Boy in CS body armor");
    set_long(
        "A compact, muscular humanoid with canine features under a Coalition\n"
        "Dead Boy helmet pushed back on his skull.  His ears twitch constantly,\n"
        "scanning for sounds outside the range of human hearing.  He carries a\n"
        "CP-40 laser rifle and wears the standard CS Dead Boy armor with the\n"
        "skull logo across the chest plate.  His eyes are unnervingly yellow.");
    set_level(7);
    set_max_hp(90);
    set_hp(90);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("dog boy");
    set_alignment(-50);
    set_body_type("humanoid");
    set_property("mdc_creature", 1);
    set_stats("MDC", 35);
    set_stats("max_MDC", 35);
    set_stats("SDC", 20);
    set_stats("max_SDC", 20);

    add_response("hello", "The Dog Boy's nose twitches.  'Credentials.  Now.'");
    add_response("hi",    "The Dog Boy's nose twitches.  'Credentials.  Now.'");
    add_response("pass",  "The Dog Boy says flatly, 'No unauthorized personnel past this point.'");
    add_response("lone star",  "The Dog Boy's jaw tightens.  'This is a restricted research installation.  Move along.'");
    add_response("science",    "The Dog Boy sniffs the air in your direction.  'Are you authorized?'");
    add_response("dog",  "The Dog Boy's lip curls.  'Say that again.'");
    add_response("bye",  "The Dog Boy goes back to scanning the perimeter, ignoring you.");
    set_default_response(
        "The Dog Boy watches you without blinking.  His ears rotate slightly toward you.");
    call_out("create_equipment", 0);
}

void init() {
    ::init();
}

void create_equipment() {
    object armor;
    object rifle;

    armor = clone_object(EQ_PATH + "dead_boy_armor.c");
    if(armor) armor->move(this_object());
    rifle = clone_object(EQ_PATH + "cp40_laser.c");
    if(rifle) rifle->move(this_object());
    call_out("equip_gear", 0);
}

void equip_gear() {
    force_me("wear dead boy armor");
    force_me("wield rifle in right hand");
}
