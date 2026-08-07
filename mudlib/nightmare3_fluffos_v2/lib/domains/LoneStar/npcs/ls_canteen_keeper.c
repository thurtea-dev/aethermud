/* /domains/LoneStar/npcs/ls_canteen_keeper.c
   Keeper of The Dry Kennel, the Lone Star support town canteen.
   Vendor: food, water, and basic field consumables at civilian prices.
   Not hostile unless attacked. */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("canteen keeper");
    set_npc_name("the keeper");
    set_id( ({ "canteen keeper", "keeper", "barkeep", "bartender",
               "proprietor", "human" }) );
    set_short("the keeper of the Dry Kennel");
    set_long(
        "A heavy-set human working the salvaged counter with a towel over one\n"
        "shoulder. One sleeve is pinned up at the elbow, and the arm that is\n"
        "left does everything without hurrying. They have run this room long\n"
        "enough to know which crews are talking and which ones are not, and\n"
        "they have never once repeated either.\n"
        "\nType 'list' for what is on. 'buy <item>' to order. Credits only.");
    set_level(5);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(50);
    set_body_type("human");
    set_property("position_str", "works the counter with a towel over one shoulder.");

    add_response("hello",
        "The keeper sets a glass down in front of you. 'List is on the "
        "board. Credits, not scrip.'");
    add_response("hi",
        "The keeper sets a glass down in front of you. 'List is on the "
        "board. Credits, not scrip.'");
    add_response("kennel",
        "'The name is a joke the first crew made and nobody has been brave "
        "enough to change it,' the keeper says.");
    add_response("patches",
        "'One per crew,' the keeper says, not looking at the wall. 'I pin "
        "them up when they rotate out. I do not take any of them down.'");
    add_response("arm",
        "'Complex accident, year twelve of the contract,' the keeper says. "
        "'They paid the settlement on time. I will give them that.'");
    add_response("lone star",
        "'Everyone in this room works for it and nobody in this room talks "
        "about it,' the keeper says. 'Keep to that and you are welcome.'");
    add_response("lab",
        "The keeper wipes the counter. 'Not in here.'");
    add_response("dog boy",
        "'Handlers drink here. Their units do not,' the keeper says. 'Not "
        "my rule.'");
    add_response("bye",
        "The keeper lifts the towel in a short wave and moves down the "
        "counter.");
    set_default_response(
        "The keeper wipes the counter and waits for you to order.");

    set_shop_items(([
        "water"         : ({ EQ_PATH+"purified_water.c",  60, "clean water, by the bottle" }),
        "bottle"        : ({ EQ_PATH+"purified_water.c",  60, "clean water, by the bottle" }),
        "drink"         : ({ EQ_PATH+"purified_water.c",  60, "clean water, by the bottle" }),
        "meal"          : ({ EQ_PATH+"rations.c",         40, "a hot plate off the counter" }),
        "food"          : ({ EQ_PATH+"rations.c",         40, "a hot plate off the counter" }),
        "rations"       : ({ EQ_PATH+"rations.c",         40, "a hot plate off the counter" }),
        "first aid kit" : ({ EQ_PATH+"first_aid_kit.c",  260, "first aid kit, off the shelf" }),
        "medkit"        : ({ EQ_PATH+"first_aid_kit.c",  260, "first aid kit, off the shelf" })
    ]));
}
