// /domains/Praxis/areas/splynn/alvurron/alvurron_temple.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, Obsidian Temple");
    set_long(
        "A temple hewn from a single mass of obsidian. Rune-carved pillars\n"
        "support a ceiling lost in shadow. Something watches from the dark.\n"
        "The plaza is west. A passage east leads deeper into the city.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/alvurron/alvurron_square",
        "east" : "/domains/Praxis/areas/splynn/alvurron/alvurron_inner_district"
    ]) );
}

void reset() {
    ::reset();
    if(!present("deevil scout", this_object()))
        clone_object("/domains/Praxis/monsters/deevil_scout")->move(this_object());
    if(!present("lesser demon", this_object()))
        clone_object("/domains/Praxis/monsters/minor_demon")->move(this_object());
}
