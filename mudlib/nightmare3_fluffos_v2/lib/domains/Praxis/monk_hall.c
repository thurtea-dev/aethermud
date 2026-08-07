#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Collapsed monk hall");
    set_long(
        "The east wing of the old monastery caved in during a ley storm.\n"
        "Broken benches and shattered stained glass litter the floor.\n"
        "A voting alcove lies east. The attic is west through a boarded door.");
    set_exits( ([
        "west" : "/domains/Praxis/monk_join",
        "east" : "/domains/Praxis/monk_vote"
    ]) );
}
