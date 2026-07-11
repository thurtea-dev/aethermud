// /domains/Praxis/areas/splynn/alvurron/alvurron_gate.c
// Alvurron: gargoyle city gate beneath Splynn.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Alvurron, City Gate");
    set_long(
        "A vaulted stone gate carved with gargoyle faces. The air is cool\n"
        "and smells of old stone and ozone. Gargoyle guards watch from\n"
        "ledges above. The city proper lies north. A residential quarter\n"
        "lies west.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/splynn_south_road",
        "north" : "/domains/Praxis/areas/splynn/alvurron/alvurron_square",
        "west"  : "/domains/Praxis/areas/splynn/alvurron/alvurron_quarter"
    ]) );
}

void reset() {
    ::reset();
    if(!present("gargoyle raider", this_object()))
        clone_object("/domains/Praxis/monsters/gargoyle_raider")->move(this_object());
}
