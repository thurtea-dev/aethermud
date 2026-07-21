/* /domains/PuertoAngel/areas/pa_outskirts.c
   Puerto Angel Outskirts -- coastal town approaches, strange and quiet. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Puerto Angel Outskirts");
    set_long(
        "The coastal town glitters ahead in the Pacific light. From a distance\n"
        "it looks almost normal, white-painted walls, terracotta rooftops, a\n"
        "harbor visible beyond. Up close the impression fractures: shuttered\n"
        "windows, an unnatural silence, no movement in daylight. The air smells\n"
        "of salt and something older.\n\n"
        "The beach lies east. The road leads north into town.");
    set_exits( ([
        "north" : "/domains/PuertoAngel/areas/pa_main_street",
        "east"  : "/domains/PuertoAngel/areas/pa_beach",
        "south" : "/domains/ChiTown/areas/illinois_road"
    ]) );
    set_listen("default",
        "Waves. Wind. Nothing else. No dogs, no birds, no voices.");
    set_smell("default",
        "Salt air, bougainvillea, and something faint that you cannot name.");
    set_items( ([
        "town"     : "Intact and maintained. More maintained than it should be, "
                     "for a town this silent.",
        "windows"  : "Shuttered. Tight. Not abandoned -- sealed.",
        "harbor"   : "Visible from here. Boats at anchor. No one on the docks.",
        "road"     : "Clean cobblestone. Someone maintains it.",
        "rooftops" : "Red terracotta. Old Mexico, perfectly preserved."
    ]) );
}

void reset() { ::reset(); }
