/* /domains/wizards/lounge_pet_trader.c
   Zera, animal handler in the Wizard's Lounge.
   Sells trained animals for wizard use and testing.
   Commands: list, buy <item> */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();
    set_name("Zera");
    set_npc_name("Zera");
    set_id( ({ "zera", "animal handler", "pet trader", "handler",
               "trader", "woman" }) );
    set_short("Zera, animal handler");
    set_long(
        "A weathered woman in a canvas vest, with a thick leather gauntlet\n"
        "on her right forearm. A perch behind her holds a spare hawk that\n"
        "watches the room with predator stillness. She deals in trained\n"
        "animals suited to wilderness and combat use.\n"
        "Type 'list' to see available animals. Type 'buy <item>' to purchase.\n"
        "Hawk commands: release hawk, call hawk, send hawk <target>, hawk scout.\n"
    );
    set_level(4);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(150);
    set_body_type("human");
    set_property("position_str", "stands by the animal perch.");

    add_response("hello", "Zera says: Looking for a companion? These are well trained.");
    add_response("hi", "Zera says: Looking for a companion? These are well trained.");
    add_response("list", "Zera says: Type 'list' to see what I have.");
    add_response("buy", "Zera says: Type 'buy <item>' to purchase. They bond on purchase.");
    add_response("hawk", "Zera says: Commands are in the hawk's description. Read it after buying.");
    add_response("falconry", "Zera says: The trained hawk responds best with the Falconry skill.");
    add_response("sell", "Zera says: I don't buy animals back. It's not fair to them.");
    add_response("bye", "Zera says: Take good care of them.");
    add_response("help", "Zera says: 'list' shows stock. 'buy <name>' purchases one.");
    set_default_response("Zera glances at you and returns her attention to the perch.");
    set_shop_sells(0);

    set_shop_items( ([
        "trained hawk"  : ({ EQ_PATH+"trained_hawk.c",      5000, "trained hunting hawk" }),
        "hawk"          : ({ EQ_PATH+"trained_hawk.c",      5000, "trained hunting hawk" }),
        "hunting hawk"  : ({ EQ_PATH+"trained_hawk.c",      5000, "trained hunting hawk" }),
        "majestic hawk" : ({ EQ_PATH+"new_trained_hawk.c",  3000, "majestic hawk" }),
        "new hawk"      : ({ EQ_PATH+"new_trained_hawk.c",  3000, "majestic hawk" })
    ]) );
}
