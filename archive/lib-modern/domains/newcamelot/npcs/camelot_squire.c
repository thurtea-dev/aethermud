/* /domains/newcamelot/npcs/camelot_squire.c
   a Camelot squire. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("camelot squire");
    set_npc_name("Squire");
    set_id( ({ "camelot squire", "squire" }) );
    set_short("a Camelot squire");
    set_long(
        "A teenager in a padded gambeson two sizes too big, drilling sword\n"
        "forms with absolute concentration and mixed results. One day this\n"
        "is a Dragon Knight. Today it is mostly bruises.");
    set_level(2);
    set_max_hp(40);
    set_hp(40);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(400);
    set_body_type("human");
    add_response("hello",
        "Squire says: 'Oh! Hello. Watch this. No wait, don't watch this.'");
    add_response("knight",
        "Squire says: 'Seven years a squire, then the vigil, then the dragon chooses or it doesn't.'");
    add_response("bye",
        "Squire says: 'Wish me luck at the practical!'");

    set_default_response("The squire runs another sword form, tongue between teeth.");
}
