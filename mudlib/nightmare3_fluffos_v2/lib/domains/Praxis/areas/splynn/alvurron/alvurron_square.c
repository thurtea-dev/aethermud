// /domains/Praxis/areas/splynn/alvurron/alvurron_square.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Alvurron, Stone Plaza");
    set_long(
        "A broad plaza of black stone lit by guttering witch-lamps. Gargoyles\n"
        "perch on every cornice. A temple of carved obsidian rises east.\n"
        "The gate is south. A gargoyle quarter opens west. A row of trading\n"
        "stalls runs north.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/alvurron/alvurron_gate",
        "east"  : "/domains/Praxis/areas/splynn/alvurron/alvurron_temple",
        "west"  : "/domains/Praxis/areas/splynn/alvurron/alvurron_quarter",
        "north" : "/domains/Praxis/areas/splynn/alvurron/alvurron_market",
        "up"    : "/domains/Praxis/areas/splynn/alvurron/alvurron_roost"
    ]) );
}

void reset() {
    ::reset();
    if(!present("brodkil", this_object()))
        clone_object("/domains/Praxis/monsters/brodkil")->move(this_object());
    if(!present("gargoyle watcher", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_watcher")->move(this_object());
}
