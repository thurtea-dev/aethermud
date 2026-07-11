// /domains/tolkeen/areas/tolkeen_tw_workshop.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Techno-Wizard Workshop");
    set_long(
        "Benches covered in etched tools, e-clips, and half-finished charms.\n"
        "A containment circle on the floor glows faintly. Signs warn against\n"
        "touching anything that hums.\n\n"
        "The mage quarter is west.");
    set_exits( ([
        "west" : "/domains/tolkeen/areas/tolkeen_mage_quarter"
    ]) );
    set_listen("default", "Sparks and a containment field buzz.");
    set_smell("default", "Ozone and solder.");
}

void reset() {
    ::reset();
    if(!present("techno-wizard vendor", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_tw_vendor")->move(this_object());
}
