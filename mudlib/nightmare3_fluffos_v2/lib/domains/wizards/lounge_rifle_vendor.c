/* /domains/wizards/lounge_rifle_vendor.c
   Vasquez, weapons testing armorer in the Wizard's Lounge.
   Sells energy rifles and sidearms for wizard combat testing.
   Commands: list, buy <item> */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();
    set_name("Vasquez");
    set_npc_name("Vasquez");
    set_id( ({ "vasquez", "armorer", "weapons armorer", "rifle vendor",
               "vendor", "woman" }) );
    set_short("Vasquez, weapons armorer");
    set_long(
        "A compact woman in a technician's vest covered in tool pockets.\n"
        "She maintains a steady supply of energy weapons for the wizard\n"
        "lounge's testing needs. Her workbench holds a disassembled CP-40\n"
        "and a collection of e-clips in various charge states.\n"
        "Type 'list' to see available weapons. Type 'buy <item>' to purchase.\n"
    );
    set_level(5);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(100);
    set_body_type("human");
    set_property("position_str", "stands at the weapons bench.");

    add_response("hello", "Vasquez says: Need something to test? Check the list.");
    add_response("hi", "Vasquez says: Need something to test? Check the list.");
    add_response("list", "Vasquez says: Type 'list' to see my stock.");
    add_response("buy", "Vasquez says: Type 'buy <item>' to purchase.");
    add_response("sell", "Vasquez says: I only supply, I don't buy back. Not my department.");
    add_response("repair", "Vasquez says: Talk to Griz in Praxis for repairs.");
    add_response("bye", "Vasquez says: Test safe.");
    add_response("help", "Vasquez says: 'list' shows stock. 'buy <name>' purchases.");
    set_default_response("Vasquez glances up briefly and goes back to her work.");
    set_shop_sells(0);

    set_shop_items( ([
        "cp-40"      : ({ EQ_PATH+"cp40_laser.c",    12000, "CP-40 pulse laser rifle" }),
        "cp40"       : ({ EQ_PATH+"cp40_laser.c",    12000, "CP-40 pulse laser rifle" }),
        "cp-30"      : ({ EQ_PATH+"cp30_laser.c",     6000, "CP-30 heavy laser pistol" }),
        "cp30"       : ({ EQ_PATH+"cp30_laser.c",     6000, "CP-30 heavy laser pistol" }),
        "c-18"       : ({ EQ_PATH+"c18_pistol.c",     4500, "C-18 laser pistol" }),
        "c18"        : ({ EQ_PATH+"c18_pistol.c",     4500, "C-18 laser pistol" }),
        "kittani"    : ({ EQ_PATH+"kittani_laser.c",  9000, "Kittani plasma rifle" }),
        "kittani laser" : ({ EQ_PATH+"kittani_laser.c", 9000, "Kittani plasma rifle" }),
        "ngr"        : ({ EQ_PATH+"ngr_rail_gun.c",  18000, "NGR rail gun" }),
        "rail gun"   : ({ EQ_PATH+"ngr_rail_gun.c",  18000, "NGR rail gun" }),
        "ngr rail gun" : ({ EQ_PATH+"ngr_rail_gun.c", 18000, "NGR rail gun" })
    ]) );
}
