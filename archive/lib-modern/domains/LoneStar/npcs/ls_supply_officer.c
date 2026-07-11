/* /domains/LoneStar/npcs/ls_supply_officer.c
   Lone Star supply depot officer.
   Sells Coalition standard field gear to any paying customer with proper papers.
   Not hostile unless attacked. */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("supply officer");
    set_npc_name("supply officer");
    set_id( ({ "supply officer", "officer", "soldier", "cs supply", "ls supply",
               "quartermaster", "coalition supply" }) );
    set_short("a Coalition supply officer");
    set_long(
        "A Coalition supply officer in standard grey duty uniform, dog tags\n"
        "visible at the collar. They manage the Lone Star depot with the\n"
        "kind of methodical calm that comes from processing requisitions for\n"
        "years without incident. They do not ask questions beyond what the\n"
        "transaction requires.\n"
        "\nType 'list' for available gear. 'buy <item>' to purchase. Credits only.");
    set_level(4);
    set_max_hp(70);
    set_hp(70);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(100);
    set_body_type("human");
    set_property("coalition_type", "cs");

    add_response("hello",
        "The officer says: 'Supply depot. What do you need?'");
    add_response("hi",
        "The officer says: 'Depot is open. List for inventory, buy for purchase.'");
    add_response("lone star",
        "The officer says: 'Lone Star Genetics Complex. Class IV restricted. "
        "You are already inside. Keep moving.'");
    add_response("research",
        "The officer says: 'Above your clearance level and mine. "
        "I supply the depot. What goes on in the lab is not my concern.'");
    add_response("dog boy",
        "The officer says: 'Dog Boys are Coalition personnel. "
        "Treat them accordingly.'");
    add_response("buy",
        "The officer says: 'Buy <item> from the list. Credits accepted. "
        "All transactions are logged.'");
    add_response("bye",
        "The officer gives a short nod. 'Move along.'");
    set_default_response(
        "The officer processes a form without looking up. 'Depot is open. "
        "List for inventory. Anything else?'");
    set_property("position_str", "reviews requisition forms behind the counter.");

    set_shop_items(([
        "e-clip"        : ({ EQ_PATH+"e_clip.c",          300, "standard e-clip (30 shots)" }),
        "eclip"         : ({ EQ_PATH+"e_clip.c",          300, "standard e-clip (30 shots)" }),
        "clip"          : ({ EQ_PATH+"e_clip.c",          300, "standard e-clip (30 shots)" }),
        "long e-clip"   : ({ EQ_PATH+"e_clip_long.c",500, "long e-clip (60 shots)" }),
        "long clip"     : ({ EQ_PATH+"e_clip_long.c",500, "long e-clip (60 shots)" }),
        "first aid kit" : ({ EQ_PATH+"first_aid_kit.c",   200, "first aid kit" }),
        "medkit"        : ({ EQ_PATH+"first_aid_kit.c",   200, "first aid kit" }),
        "rations"       : ({ EQ_PATH+"rations.c",          50, "field rations" }),
        "ration"        : ({ EQ_PATH+"rations.c",          50, "field rations" }),
        "food"          : ({ EQ_PATH+"rations.c",          50, "field rations" }),
        "rope"          : ({ EQ_PATH+"rope.c",            100, "50 feet of rope" })
    ]));
}
