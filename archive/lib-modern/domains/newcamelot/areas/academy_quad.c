/* /domains/newcamelot/areas/academy_quad.c
   Royal Academy of Magic - Quadrangle. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Royal Academy of Magic - Quadrangle");
    set_long(
        "The quadrangle of the Royal Academy of Magic. Lawns squared off\n"
        "with gravel paths surround a fountain that flows upward, the\n"
        "water climbing lazily into a shape that is almost a tree. The\n"
        "lecture hall is north, the library southeast, the dormitory west.\n"
        "The city wizard tower lies east along a lane. A stair behind the\n"
        "fountain leads down to the summoning chamber.");
    set_listen("default",
        "The upward fountain burbling, and a distant lecture drone.");
    set_smell("default",
        "Cut grass and a whiff of ozone.");
    set_items( ([
        "fountain" : "The water flows up. Nobody explains it; first-years are expected to work it out.",
        "lawns" : "Trimmed by animated shears, which are having a wonderful time.",
        "paths" : "White gravel, raked in patterns that shift weekly."
    ]) );
    set_exits( ([
        "east" : "/domains/newcamelot/areas/camelot_wizard_tower",
        "north" : "/domains/newcamelot/areas/academy_lecture_hall",
        "west" : "/domains/newcamelot/areas/academy_dormitory",
        "southeast" : "/domains/newcamelot/areas/academy_library",
        "down" : "/domains/newcamelot/areas/academy_summoning"
    ]) );
}

void reset() {
    ::reset();
    if(!present("academy student", this_object()))
        new("/domains/newcamelot/npcs/academy_student")->move(this_object());
}
