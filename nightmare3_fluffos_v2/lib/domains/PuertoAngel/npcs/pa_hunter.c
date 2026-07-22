/* /domains/PuertoAngel/npcs/pa_hunter.c
   Elena Vasquez -- independent vampire hunter living in Puerto Angel.
   She has an arrangement with the older vampires: she kills the wild ones
   that arrive from outside and the town does not kill her.
   Located in La Cantina. Source of information about vampires. */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/PuertoAngel/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("Elena");
    set_npc_name("Elena");
    set_id( ({ "elena", "elena vasquez", "hunter", "vampire hunter",
               "vasquez", "woman", "human" }) );
    set_short("Elena Vasquez, vampire hunter");
    set_long(
        "A woman in her mid-thirties with weathered skin and the particular\n"
        "stillness of someone who has spent years not moving when she should\n"
        "not. She nurses a drink she has not finished in two hours. Silver\n"
        "rings on every finger and a cross visible at her collar. The\n"
        "vampires in the cantina watch her the way they watch everything:\n"
        "without urgency, without blinking. The truce holds because both\n"
        "sides find it useful.");
    set_level(7);
    set_max_hp(110);
    set_hp(110);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(200);
    set_body_type("human");
    set_property("position_str", "sits at a corner table, watching the room.");

    set_shop_items(([
        "silver cross"  : ({ EQ_PATH+"silver_cross.c",     250, "a heavy silver cross" }),
        "cross"         : ({ EQ_PATH+"silver_cross.c",     250, "a heavy silver cross" }),
        "wooden stake"  : ({ EQ_PATH+"wooden_stake.c",       25, "a sharpened wooden stake" }),
        "stake"         : ({ EQ_PATH+"wooden_stake.c",       25, "a sharpened wooden stake" }),
        "silver dagger" : ({ EQ_PATH+"silver_dagger.c",     350, "a silver dagger" }),
        "dagger"        : ({ EQ_PATH+"silver_dagger.c",     350, "a silver dagger" }),
        "holy water"    : ({ EQ_PATH+"holy_water_vial.c",    60, "a vial of holy water" }),
        "vial"          : ({ EQ_PATH+"holy_water_vial.c",    60, "a vial of holy water" })
    ]));

    add_response("hello",
        "Elena says: 'Sit down. If you are here it means you came through "
        "the town and no one stopped you. That is either good judgment "
        "on your part or very bad luck.'");
    add_response("hi",
        "Elena says: 'Hello. You are alive. So far so good.'");
    add_response("vampire",
        "Elena says: 'I know more about them than anyone in this cantina. "
        "The old ones here are stable. Territorial but not predatory -- not toward "
        "visitors who do not cause trouble. The wild ones that come in from outside "
        "are different. Those I deal with.'");
    add_response("wild vampire",
        "Elena says: 'Younger. Undisciplined. No arrangement with the town, "
        "no reason to hold back. They come in through the harbor mostly. "
        "At night. Silver hurts them. Running water stops them cold. "
        "Fire works best but tends to cause other problems.'");
    add_response("weakness",
        "Elena says: 'Silver -- carry it. Running water they cannot cross. "
        "Sunlight does not destroy them the way stories say, but it hurts. "
        "Holy symbols work if you mean it -- dead ones in your hand are "
        "just metal. The vampires here will not tell you this. I will.'");
    add_response("arrangement",
        "Elena says: 'I kill the ones that come in wild. The ones who live here "
        "do not try to kill me. Six years. It has held this long.'");
    add_response("silver",
        "Elena says: 'The church has a cross. Real silver, old. "
        "The vampires do not touch it. Take it if you plan to go near the harbor "
        "after dark -- something hunts there that the town has not fully "
        "contained.'");
    add_response("church",
        "Elena says: 'North of the residential district. The vampires leave it "
        "alone. I do not know why -- they will not say. But they do. "
        "There is a silver cross on the altar. Take it if you need it.'");
    add_response("harbor",
        "Elena says: 'Do not go there at night alone. Something has been coming "
        "in off the water -- wild vampires, young, following the boats. "
        "The town has not sorted it out yet. That is what I am working on.'");
    add_response("town",
        "Elena says: 'Puerto Angel is theirs. Has been since the Rifts. "
        "The old ones built something here -- stable, careful, not what "
        "you expect. Do not mistake careful for safe.'");
    add_response("danger",
        "Elena says: 'The cantina is the safest place in town. "
        "The older vampires keep order here. Outside after dark, "
        "especially near the harbor, is something else entirely.'");
    add_response("help",
        "Elena says: 'Kill the wild ones if they come at you. "
        "Do not touch the ones who live here unless they move first. "
        "And carry silver. Always.'");
    add_response("bye",
        "Elena says: 'Watch the harbor road. And do not run -- "
        "they track movement. Walk like you belong here and you might.'");
    set_default_response(
        "Elena watches the room for a moment before answering. "
        "'Ask me about vampires, silver, the church, or the harbor. "
        "That is what I know.'");
}

void init() {
    ::init();
}
