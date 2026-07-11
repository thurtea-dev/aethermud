// /domains/wizards/hallway.c
// The wizard hallway. Doors lead to individual workrooms.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("the wizard hallway");
    set_long(
        "A long corridor of polished stone. Doors line both walls, each marked\n"
        "with a small nameplate. Each door leads to a wizard workroom. The air\n"
        "hums faintly with contained magical potential. A wider passage at the\n"
        "south end opens into the Wizard's Lounge.\n"
    );
    set_exits( ([
        "out"   : "/domains/Praxis/areas/monument_square",
        "south" : "/domains/wizards/lounge"
    ]) );
}

void add_wiz_exit(string name, string dest) {
    string *current;
    mapping ex;
    int i;

    current = query_exits();
    ex = ([]);
    if(current)
        for(i = 0; i < sizeof(current); i++)
            ex[current[i]] = query_exit(current[i]);
    ex[lower_case(name)] = dest;
    set_exits(ex);
}
