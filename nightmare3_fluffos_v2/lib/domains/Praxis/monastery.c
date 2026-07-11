/* /domains/Praxis/monastery.c
 * Ruined chapel, post-Rifts Praxis.
 */
#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_short("a ruined Praxis chapel");
    set_long(
        "Stone walls stand half-collapsed around what was once a place of worship.\n"
        "Rifts-era energy discharges have scorched the remaining masonry, and the\n"
        "floor is thick with ash and rubble. The space is quiet now.");
    set_exits(
        (["south" : "/domains/Praxis/e_boc_la1",
          "east"  : "/domains/Praxis/stairs",
          "north" : "/domains/Praxis/chapel"]) );
    set_items(
        (["walls"  : "Scorched stone, cracked by heat or energy discharge.",
          "rubble" : "Fragments of masonry and ash from whatever destroyed this place.",
          "floor"  : "Compacted ash and broken stone cover what remains of the floor."]) );
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_property("no castle", 1);
}
