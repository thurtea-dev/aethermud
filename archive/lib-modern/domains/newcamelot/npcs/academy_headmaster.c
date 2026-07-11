/* /domains/newcamelot/npcs/academy_headmaster.c
   the Academy headmaster. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("academy headmaster");
    set_npc_name("Headmaster");
    set_id( ({ "academy headmaster", "headmaster", "professor", "master" }) );
    set_short("the Academy headmaster");
    set_long(
        "A silver-haired mage in formal robes of midnight blue, chalk dust\n"
        "on both sleeves. He has the air of a man who has personally\n"
        "banished more demons than he has passed students, and considers\n"
        "the ratio about right.");
    set_level(18);
    set_max_hp(500);
    set_hp(500);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("elf");
    set_alignment(600);
    set_body_type("human");
    add_response("hello",
        "Headmaster says: 'A visitor. Auditing, or enrolling? The paperwork differs considerably.'");
    add_response("magic",
        "Headmaster says: 'Magic is a ley line and a decimal point. Respect both.'");
    add_response("mrrlyn",
        "The headmaster's face goes carefully, completely neutral. 'The King's counselor is not faculty.'");
    add_response("bye",
        "Headmaster says: 'Do close the door. The third blackboard escapes.'");

    set_default_response("The headmaster grades essays with terrifying speed.");
}
