// /domains/chitown/npcs/cs_outfitter.c

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("CS outfitter");
    set_npc_name("Outfitter");
    set_id( ({ "cs outfitter", "outfitter", "vendor", "tailor" }) );
    set_short("a CS outfitter");
    set_long(
        "A Coalition-licensed outfitter selling approved civilian clothing\n"
        "and light gear. Everything bears a CS product seal.\n"
        "\nType 'list' or 'buy <item>'.");
    set_level(2);
    set_max_hp(40);
    set_hp(40);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(50);
    set_body_type("human");

    add_response("hello", "Outfitter: 'Approved wear only. List to see stock.'");
    set_default_response("The outfitter folds a gray jacket and waits.");

    set_shop_items(([
        "leather jacket" : ({ EQ_PATH+"leather_jacket.c", 900, "leather jacket" }),
        "jacket" : ({ EQ_PATH+"leather_jacket.c", 900, "leather jacket" }),
        "rope" : ({ EQ_PATH+"rope.c", 120, "rope" }),
        "backpack" : ({ EQ_PATH+"backpack.c", 200, "backpack" }),
        "pack" : ({ EQ_PATH+"backpack.c", 200, "backpack" }),
        "rebreather" : ({ EQ_PATH+"rebreather.c", 1200, "rebreather mask" }),
        "mask" : ({ EQ_PATH+"rebreather.c", 1200, "rebreather mask" })
    ]));
}
