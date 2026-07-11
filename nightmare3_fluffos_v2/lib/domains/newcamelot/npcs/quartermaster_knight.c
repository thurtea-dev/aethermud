/* /domains/newcamelot/npcs/quartermaster_knight.c
   the knight quartermaster. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("knight quartermaster");
    set_npc_name("Quartermaster");
    set_id( ({ "knight quartermaster", "quartermaster", "knight" }) );
    set_short("the knight quartermaster");
    set_long(
        "A greying Dragon Knight who traded the patrol rota for the armory\n"
        "ledger and treats every blade in the vault as a sworn charge. One\n"
        "eye is cybernetic and it never stops moving.");
    set_level(14);
    set_max_hp(300);
    set_hp(300);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(700);
    set_body_type("human");
    add_response("hello",
        "Quartermaster says: 'Look all you like. Touch the lance and we will have a conversation.'");
    add_response("lance",
        "Quartermaster says: 'Dragonfire lance. Forged in an actual dragon's flame, blessed by the Order. It is not for sale, borrowing, or heroic emergencies.'");
    add_response("bye",
        "Quartermaster says: 'Mind the step on the way out.'");

    set_default_response("The quartermaster polishes a shield that is already spotless.");
}
