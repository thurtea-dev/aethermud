/* /domains/ChiTown/areas/chitown_collapsed_nest.c
   The collapsed void above Dead End Junction. Whatever has been
   dripping and scratching down through the ceiling lives here. */

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/ChiTown/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_short("Collapsed Nest");
    set_long(
        "A pocket of open space above the tunnels, opened up whenever this\n"
        "section of Chi-Town's substructure gave way. Broken rebar and\n"
        "shattered concrete slabs lean at every angle. Something has\n"
        "dragged scraps of insulation and torn cabling into a rough heap\n"
        "against the far side, matted down by long use.\n\n"
        "The gap back down to the junction is the only way out.");
    set_exits( ([
        "down" : "/domains/ChiTown/areas/chitown_dead_end_junction"
    ]) );
    set_listen("default",
        "Breathing, close and slow, from somewhere in the dark above the\n"
        "rebar.");
    set_smell("default",
        "Wet rust and the same faintly organic smell from the junction\n"
        "below, stronger here.");
    set_items( ([
        "rebar"  : "Broken rebar at every angle, some of it bent enough to\n"
                   "have been used as handholds.",
        "slabs"  : "Shattered concrete slabs, fallen at odd angles when this\n"
                   "pocket opened up.",
        "heap"   : "Insulation and torn cabling, dragged into a rough nest\n"
                   "and matted down by long use."
    ]) );
}

void reset() {
    ::reset();
    if(!present("junction stalker", this_object()))
        clone_object(MONSTER_PATH+"chitown_junction_stalker.c")->move(this_object());
}
