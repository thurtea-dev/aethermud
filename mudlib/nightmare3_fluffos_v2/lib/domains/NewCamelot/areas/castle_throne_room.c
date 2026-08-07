/* /domains/NewCamelot/areas/castle_throne_room.c
   Camelot Castle - Throne Room. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Camelot Castle - Throne Room");
    set_long(
        "The throne room of New Camelot. A high seat of white stone and\n"
        "gold stands on a dais beneath a window shaped like a rising sun.\n"
        "The floor is a single sheet of polished marble, and the air hums\n"
        "faintly, as if the room itself is listening. The great hall is\n"
        "south.");
    set_listen("default",
        "A faint hum, just below hearing.");
    set_smell("default",
        "Cold marble and a trace of ozone.");
    set_items( ([
        "throne" : "White stone chased with gold. It manages to look both ancient and impossibly new.",
        "window" : "A rising sun in amber glass. At dawn it sets the whole room alight.",
        "dais" : "Three broad steps of marble."
    ]) );
    set_exits( ([
        "south" : "/domains/NewCamelot/areas/castle_great_hall"
    ]) );
}

void reset() {
    ::reset();
    if(!present("king", this_object()))
        new("/domains/NewCamelot/npcs/king_arrthuu")->move(this_object());
    if(!present("mrrlyn", this_object()))
        new("/domains/NewCamelot/npcs/mrrlyn")->move(this_object());
}
