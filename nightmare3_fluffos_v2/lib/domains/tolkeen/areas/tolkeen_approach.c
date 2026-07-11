// /domains/tolkeen/areas/tolkeen_approach.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Southern Approach");
    set_long(
        "A scarred road rises toward Tolkeen's mage gate. Burned vehicles\n"
        "line the ditches. Ward markers warn travelers: Coalition scouts\n"
        "hunt this stretch after dark - and sometimes before.\n\n"
        "The mage gate is north. Scorched fields continue south.");
    set_exits( ([
        "north" : "/domains/tolkeen/areas/tolkeen_gate",
        "south" : "/domains/tolkeen/areas/tolkeen_scorched_field"
    ]) );
    set_listen("default", "Wind through wreckage. Distant wall horns.");
    set_smell("default", "Ash and old fuel.");
}
