// /domains/ChiTown/npcs/cs_arms_clerk.c
// Licensed civilian arms (lighter than quartermaster military stock).

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("arms clerk");
    set_npc_name("Arms Clerk");
    set_id( ({ "arms clerk", "clerk", "vendor", "gunsmith" }) );
    set_short("an arms clerk");
    set_long(
        "A licensed Coalition arms dealer behind armored glass. Civilian\n"
        "permits only. Military hardware is handled at the armory.\n"
        "\nType 'list' or 'buy <item>'.");
    set_level(4);
    set_max_hp(50);
    set_hp(50);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(-50);
    set_body_type("human");

    add_response("hello", "Clerk: 'Permit and credits. List shows approved stock.'");
    add_response("hi", "Clerk: 'Permit and credits. List shows approved stock.'");
    set_default_response("The clerk taps the glass. 'Decide.'");

    set_shop_items(([
        "cp-30" : ({ EQ_PATH+"cp30_laser.c", 5500, "CP-30 laser pistol" }),
        "cp30" : ({ EQ_PATH+"cp30_laser.c", 5500, "CP-30 laser pistol" }),
        "pistol" : ({ EQ_PATH+"cp30_laser.c", 5500, "CP-30 laser pistol" }),
        "knife" : ({ EQ_PATH+"knife.c", 200, "knife" }),
        "combat knife" : ({ EQ_PATH+"knife.c", 200, "knife" }),
        "e-clip" : ({ EQ_PATH+"e_clip.c", 150, "energy clip" }),
        "clip" : ({ EQ_PATH+"e_clip.c", 150, "energy clip" })
    ]));
}
