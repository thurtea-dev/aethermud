#include <std.h>
#include <rooms.h>

inherit ROOM;

int room_107();
int room_108();

void create() {
    ::create();
    set_properties( ([ "light" : 1, "no castle" : 1 ]) );
    set_short("Praxis Refuge Inn, upper hallway");
    set_long(
        "The inn corridor bends slightly here. A window looks out over\n"
        "Praxis rooftops and the distant glow of Monument Square. Room\n"
        "doors continue down both sides.");
    set_items( ([
        "window"  : "Thick glass overlooking post-apocalyptic Praxis.",
        "hallway" : "Upper inn corridor.",
        "carpet"  : "Burgundy runner, patched in places.",
        "doors"   : "Numbered guest rooms."
    ]) );
    set_exits( ([
        "north" : "/domains/Praxis/hall5",
        "south" : "/domains/Praxis/hall3",
        "east"  : ({ "/domains/Praxis/hotelroom107", (: room_107 :) }),
        "west"  : ({ "/domains/Praxis/hotelroom108", (: room_108 :) })
    ]) );
}

int room_107() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(107), this_player()))
        return 1;
    write("The door to room 107 is locked. You need the key.\n");
    return 0;
}

int room_108() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(108), this_player()))
        return 1;
    write("The door to room 108 is locked. You need the key.\n");
    return 0;
}
