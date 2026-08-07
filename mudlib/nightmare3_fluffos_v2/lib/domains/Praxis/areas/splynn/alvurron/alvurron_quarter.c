// /domains/Praxis/areas/splynn/alvurron/alvurron_quarter.c
// Residential quarter of the gargoyle city Alvurron.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, Gargoyle Quarter");
    set_long(
        "Narrow alleys between obsidian tenements. Gargoyle families perch on\n"
        "stone balconies above. Witch-lamps cast blue-green light on carved\n"
        "lintels. The plaza lies east. A sealed stairway south leads down to\n"
        "a holding area the locals do not talk about.");
    set_exits( ([
        "east"  : "/domains/Praxis/areas/splynn/alvurron/alvurron_square",
        "south" : "/domains/Praxis/areas/splynn/alvurron/alvurron_slave_pens",
        "west"  : "/domains/Praxis/areas/splynn/alvurron/alvurron_warrens",
        "southeast" : "/domains/Praxis/areas/splynn/alvurron/alvurron_gate"
    ]) );
    set_listen("default",
        "Wings rustle on ledges. A gargoyle child laughs somewhere above.");
    set_smell("default", "Cold stone and ozone from the ley lines below.");
}

void reset() {
    ::reset();
    if(!present("gargoyle", this_object()))
        clone_object("/domains/Praxis/monsters/gargoyle")->move(this_object());
}
