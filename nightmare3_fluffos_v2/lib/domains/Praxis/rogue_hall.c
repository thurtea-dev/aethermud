#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Rogue war room");
    set_long(
        "Maps of old Praxis cover a table, stained with coffee and blood.\n"
        "Weapon racks stand empty. The rogue guild that plotted here is\n"
        "long gone. Stairs lead up to the safehouse. A voting nook is west.");
    set_exits( ([
        "up"   : "/domains/Praxis/rogue_join",
        "west" : "/domains/Praxis/rogue_vote"
    ]) );
}
