/* /domains/Praxis/fighter_hall.c
   Ruined training hall, stripped and abandoned. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_properties( (["no attack": 1, "light":2, "indoors":1]) );
    set_short("a ruined training hall");
    set_long(
        "A large chamber with cracked stone floors and bare walls where "
        "equipment once hung. The wooden training mats rotted away or "
        "were hauled off for scrap long before this place was forgotten. "
        "Scuff marks and old impact craters scar the walls, hints of "
        "what this space was built for. Whoever used it moved on and "
        "left nothing worth taking.\n"
    );
    set_exits(
        (["down" : "/domains/Praxis/fighter_join",
          "council" : "/domains/Praxis/council_hall",
          "west" : "/realms/grumpy/fighter/room/nin_remember",
          "east" : "/domains/Praxis/fighter_vote"]) );
}
