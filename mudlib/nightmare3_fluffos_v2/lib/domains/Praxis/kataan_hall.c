/* /domains/Praxis/kataan_hall.c
   Forgotten sub-basement, origin unknown. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_properties(([ "no attack":1, "light":2, "indoors":1]));
    set_short("a forgotten sub-basement");
    set_long(
        "A low-ceilinged chamber cut from bare rock, well below street "
        "level. The walls are damp and unmarked. Old crates and rotted "
        "shelving lean against one corner, contents long since removed or "
        "decomposed. No markings, no inscriptions, nothing to indicate "
        "who built this space or why. It smells of mold and stagnant "
        "air.\n"
    );
    set_exits(
        (["up" : "/domains/Praxis/kataan_join",
          "council" : "/domains/Praxis/council_hall",
          "north" : "/realms/grumpy/kataan/room/room1.c",
          "south" : "/realms/grumpy/kataan/room/storage",
          "east" : "/domains/Praxis/kataan_vote"]) );
}
