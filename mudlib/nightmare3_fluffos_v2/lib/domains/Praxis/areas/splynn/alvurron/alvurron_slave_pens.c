// /domains/Praxis/areas/splynn/alvurron/alvurron_slave_pens.c
// Gargoyle empire's own holding pens, sealed below the quarter.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_short("Alvurron, Captive Hold");
    set_long(
        "A low stone chamber walled off from the quarter above. Dimensional\n"
        "cages are stacked three high along both walls, each one humming with\n"
        "a faint containment field. This is not the Splugorth's trade. These\n"
        "captives belong to the gargoyle empire itself, kept apart and\n"
        "unlisted. A brodkil overseer moves between the rows with a data\n"
        "slate and does not acknowledge you unless you move toward the\n"
        "cages. A sealed stairway leads back up to the quarter.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/alvurron/alvurron_quarter"
    ]) );
    set_listen("default",
        "The low hum of containment fields and the occasional scrape of a\n"
        "data slate stylus.");
    set_smell("default", "Cold stone and the metallic tang of the field generators.");
    set_items( ([
        "cages"    : "Dimensional cages stacked three high, each humming with a\n"
                      "faint containment field.",
        "captives" : "Held apart from the Splugorth's trade and unlisted anywhere.\n"
                      "None of them meet your eyes.",
        "slate"    : "A data slate carried by the overseer. You cannot read the script."
    ]) );
}

void reset() {
    ::reset();
    if(!present("brodkil overseer", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_overseer")->move(this_object());
}
