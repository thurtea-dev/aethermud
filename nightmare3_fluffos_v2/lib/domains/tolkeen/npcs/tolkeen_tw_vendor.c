// /domains/tolkeen/npcs/tolkeen_tw_vendor.c

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("techno-wizard vendor");
    set_npc_name("TW Vendor");
    set_id( ({ "techno-wizard vendor", "tw vendor", "vendor", "techno-wizard",
               "shopkeeper" }) );
    set_short("a techno-wizard vendor");
    set_long(
        "A techno-wizard behind a stall of etched tools and glowing scrap.\n"
        "Sparks jump between fingertips when the wards pulse.\n"
        "\nType 'list' or 'buy <item>'.");
    set_level(8);
    set_max_hp(90);
    set_hp(90);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(100);
    set_body_type("human");

    add_response("hello", "Vendor: 'Components, charms, and things that should not work - but do.'");
    set_default_response("The vendor adjusts a humming crystal.");

    set_shop_items(([
        "first aid kit" : ({ EQ_PATH+"first_aid_kit.c", 300, "field medkit" }),
        "medkit" : ({ EQ_PATH+"first_aid_kit.c", 300, "field medkit" }),
        "rations" : ({ EQ_PATH+"rations.c", 60, "field rations" }),
        "rope" : ({ EQ_PATH+"rope.c", 100, "rope" }),
        "knife" : ({ EQ_PATH+"knife.c", 180, "knife" }),
        "e-clip" : ({ EQ_PATH+"e_clip.c", 200, "energy clip" }),
        "clip" : ({ EQ_PATH+"e_clip.c", 200, "energy clip" })
    ]));
}
