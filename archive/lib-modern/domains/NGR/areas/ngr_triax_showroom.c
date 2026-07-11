/* /domains/NGR/areas/ngr_triax_showroom.c
   Triax Showroom. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Triax Showroom");
    set_long(
        "A gleaming showroom of brushed steel and glass, the retail face of\n"
        "Triax Industries. Display pedestals hold body armor, helmets and a\n"
        "rail gun under spotlights, each with a printed spec card. A sales\n"
        "representative in a crisp suit materializes at your elbow. The\n"
        "plaza is south.");
    set_exits( ([
        "south" : "/domains/NGR/areas/ngr_plaza"
    ]) );
    set_listen("default",
        "Soft corporate music and the hum of display lighting.");
    set_smell("default", "Glass cleaner and new polymer.");
    set_items( ([
        "pedestals" : "Brushed steel pedestals, each with armor or arms under
glass and spotlights.",
        "cards" : "Printed spec cards: MDC ratings, warranty terms, and a
disclaimer about gargoyle-related damage.",
        "spotlights" : "Angled to make everything look ten percent deadlier."
    ]) );
}

void reset() {
    ::reset();
    if(!present("sales representative", this_object()))
        clone_object("/domains/NGR/npcs/triax_rep")->move(this_object());
}
