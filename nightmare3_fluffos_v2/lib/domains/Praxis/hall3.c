#include <std.h>
#include <rooms.h>

inherit ROOM;

int room_105();
int room_106();

void create() {
    ::create();
    set_properties( ([ "light" : 1, "no castle" : 1 ]) );
    set_short("Praxis Refuge Inn, upper hallway");
    set_long(
        "Another stretch of the Refuge Inn upper floor. Faded Coalition\n"
        "recruitment posters cover one wall. Guest room doors stand closed\n"
        "on either side.");
    set_items( ([
        "hallway" : "Inn corridor with numbered rooms.",
        "carpet"  : "Deep burgundy, threadbare near the stairs.",
        "posters" : "Old CS recruitment flyers, curling at the corners.",
        "doors"   : "Guest room doors with card-key slots."
    ]) );
    set_exits( ([
        "north" : "/domains/Praxis/hall4",
        "south" : "/domains/Praxis/hall2",
        "east"  : ({ "/domains/Praxis/hotelroom105", (: room_105 :) }),
        "west"  : ({ "/domains/Praxis/hotelroom106", (: room_106 :) })
    ]) );
}

int room_105() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(105), this_player()))
        return 1;
    write("The door to room 105 is locked. You need the key.\n");
    return 0;
}

int room_106() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(106), this_player()))
        return 1;
    write("The door to room 106 is locked. You need the key.\n");
    return 0;
}
