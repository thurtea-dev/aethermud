// /domains/chitown/npcs/cs_ration_clerk.c

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("ration clerk");
    set_npc_name("Ration Clerk");
    set_id( ({ "ration clerk", "clerk", "vendor", "shopkeeper" }) );
    set_short("a ration clerk");
    set_long(
        "A thin human behind a CS-branded counter stacked with sealed meal\n"
        "packs. Everything is numbered. Everything is logged.\n"
        "\nType 'list' or 'buy <item>'.");
    set_level(2);
    set_max_hp(40);
    set_hp(40);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(0);
    set_body_type("human");

    add_response("hello", "Clerk: 'Ration allotments and civilian packs. List to browse.'");
    add_response("hi", "Clerk: 'Ration allotments and civilian packs. List to browse.'");
    set_default_response("The clerk waits with a scanner ready.");

    set_shop_items(([
        "rations" : ({ EQ_PATH+"rations.c", 80, "CS field rations" }),
        "ration" : ({ EQ_PATH+"rations.c", 80, "CS field rations" }),
        "food" : ({ EQ_PATH+"rations.c", 80, "CS field rations" }),
        "first aid kit" : ({ EQ_PATH+"first_aid_kit.c", 250, "civilian first aid kit" }),
        "medkit" : ({ EQ_PATH+"first_aid_kit.c", 250, "civilian first aid kit" })
    ]));
}
