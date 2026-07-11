/* /domains/Horton/areas/horton_hunting_blind.c
   Militia hunting blind east of pine hollow. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Hunting Blind");
    set_long(
        "A raised platform of scrap lumber and camouflage netting. Empty\n"
        "shell boxes and a thermos left behind. From here the cornfield and\n"
        "scrub ridge are visible on clear days.\n\n"
        "Pine hollow west.");
    set_exits( ([
        "west" : "/domains/Horton/areas/horton_pine_hollow"
    ]) );
    set_listen("default", "Birds. The creak of the platform.");
    set_smell("default", "Old coffee and pine.");
    set_items( ([
        "platform" : "Stable enough. Railings missing on one side.",
        "netting"  : "Faded green. Still works if you sit still.",
        "thermos"  : "Empty. Horton militia mark scratched on the side."
    ]) );
}

void reset() {
    ::reset();
    if(random(3) == 0 && !present("horton militia guard", this_object()))
        clone_object("/domains/Horton/monsters/horton_militia_guard")->move(this_object());
}
