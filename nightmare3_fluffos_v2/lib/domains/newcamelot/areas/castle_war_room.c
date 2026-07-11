/* /domains/newcamelot/areas/castle_war_room.c
   Camelot Castle - War Room. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Camelot Castle - War Room");
    set_long(
        "The war room of the Camelot Order. A relief map of the British\n"
        "Isles and the European coast fills the central table, studded\n"
        "with markers: brodkil warbands, Splugorth slave raids, NGR patrol\n"
        "lines. Racks of message cylinders and a bank of pre-Rifts radio\n"
        "gear line the walls. The great hall is west.");
    set_listen("default",
        "The soft crackle of a radio left on watch frequency.");
    set_smell("default",
        "Paper, ink, and radio-set warmth.");
    set_items( ([
        "map" : "Carved oak and painted plaster. The markers north of the Channel are thickest.",
        "markers" : "Red for demons, black for Splugorth, grey for the NGR.",
        "radio" : "Rebuilt pre-Rifts military sets. They still reach Calais on a clear night."
    ]) );
    set_exits( ([
        "west" : "/domains/newcamelot/areas/castle_great_hall"
    ]) );
}
