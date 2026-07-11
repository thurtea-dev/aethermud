#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Monastery voting alcove");
    set_long(
        "A small alcove where the old monastic order once held votes. The\n"
        "ballot box is rusted shut. The hall lies west.");
    set_exits( ([
        "west" : "/domains/Praxis/monk_hall"
    ]) );
}
