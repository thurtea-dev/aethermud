/* /domains/newcamelot/npcs/academy_student.c
   a Royal Academy student. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("academy student");
    set_npc_name("Student");
    set_id( ({ "academy student", "student", "apprentice" }) );
    set_short("a Royal Academy student");
    set_long(
        "A young mage in student robes, ink-stained to the elbows, hauling\n"
        "a satchel of books that is visibly heavier than the student. A\n"
        "crystal focus swings from a cord around the neck, humming softly.");
    set_level(3);
    set_max_hp(50);
    set_hp(50);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(200);
    set_body_type("human");
    add_response("hello",
        "Student says: 'Hi! Can't stop. Summoning practical in an hour and my circle geometry is a disaster.'");
    add_response("academy",
        "Student says: 'Best magic school in Europe. The library hums. That's normal. Probably.'");
    add_response("bye",
        "Student says: 'Wish me luck with the elemental!'");

    set_default_response("The student flips through a grimoire, muttering formulae.");
}
