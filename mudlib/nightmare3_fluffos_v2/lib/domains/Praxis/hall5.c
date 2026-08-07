#include <std.h>
#include <rooms.h>

inherit ROOM;

int room_109();
int room_110();

void create() {
    ::create();
    set_properties( ([ "light" : 1, "no castle" : 1 ]) );
    set_short("Praxis Refuge Inn, upper hallway");
    set_long(
        "The top-floor corridor ends here at a reinforced stairwell door.\n"
        "Guest rooms 109 and 110 flank the hall. The air smells of ozone\n"
        "from the inn's backup generator.");
    set_items( ([
        "stairwell" : "A door marked STAIRS leading down to the lobby.",
        "generator" : "You hear a faint hum from behind the walls.",
        "doors"     : "The last guest rooms on this wing."
    ]) );
    set_exits( ([
        "east"  : ({ "/domains/Praxis/hotelroom109", (: room_109 :) }),
        "west"  : ({ "/domains/Praxis/hotelroom110", (: room_110 :) }),
        "south" : "/domains/Praxis/hall4"
    ]) );
}

int room_109() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(109), this_player()))
        return 1;
    write("The door to room 109 is locked. You need the key.\n");
    return 0;
}

int room_110() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(110), this_player()))
        return 1;
    write("The door to room 110 is locked. You need the key.\n");
    return 0;
}
