// /obj/credit_chip.c
// Lootable credit chip dropped on player death.
// Contains a set amount of credits that can be taken.

#include <std.h>
#include "move.h"

inherit OBJECT;

private int __credits;

void create() {
    ::create();
    __credits = 0;
    set_name("credit chip");
    set_id(({ "credit chip", "chip", "credits" }));
    set_short("a credit chip");
    set_long(
        "A standard-issue credit chip with an embedded value counter.\n"
        "The display reads: " + __credits + " credits.");
    set_mass(10);
    set_value(__credits);
}

void set_credits(int amt) {
    __credits = (amt > 0 ? amt : 0);
    set_value(__credits);
    set_long(
        "A standard-issue credit chip with an embedded value counter.\n"
        "The display reads: " + __credits + " credits.");
    set_short("a credit chip (" + __credits + " cr)");
}

int query_credits() { return __credits; }

// When picked up, transfer credits to player and destroy this chip.
int move(mixed dest) {
    object player;
    int result;

    result = ::move(dest);
    if(result != MOVE_OK) return result;

    if(objectp(dest) && (int)dest->is_player()) {
        player = dest;
        player->add_money("credits", __credits);
        tell_object(player,
            "You pocket the credit chip (" + __credits + " credits).\n");
        call_out("remove", 0);
    }
    return result;
}
