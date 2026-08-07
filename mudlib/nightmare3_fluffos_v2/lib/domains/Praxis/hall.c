/* /d/Praxis/hall.c
   Praxis Refuge Inn, upper hallway (rooms 101 and 102).
   Guest room doors are keyed: rent at the front desk in hotel.c. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

int room_101();
int room_102();

void create() {
    ::create();
    set_properties( ([ "light" : 1, "no castle" : 1 ]) );
    set_short("Praxis Refuge Inn, upper hallway");
    set_long(
        "A quiet corridor in the Praxis Refuge Inn. Reinforced doors line\n"
        "both sides, each marked with a room number. Battery lanterns cast\n"
        "a steady glow on scuffed burgundy carpet. Room 101 is east and\n"
        "room 102 is west. The main lobby is south.");
    set_items( ([
        "hallway" : "A narrow inn corridor with numbered guest rooms.",
        "carpet"  : "Deep burgundy, worn at the edges.",
        "lanterns": "Battery-powered lanterns bolted to the walls.",
        "doors"   : "Reinforced doors with electronic locks. You need the\n"
                    "right key to enter a room."
    ]) );
    set_exits( ([
        "north" : "/domains/Praxis/hall2",
        "south" : "/domains/Praxis/hotel",
        "east"  : ({ "/domains/Praxis/hotelroom101", (: room_101 :) }),
        "west"  : ({ "/domains/Praxis/hotelroom102", (: room_102 :) })
    ]) );
}

int room_101() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(101), this_player()))
        return 1;
    write("The door to room 101 is locked. You need the key.\n");
    return 0;
}

int room_102() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(102), this_player()))
        return 1;
    write("The door to room 102 is locked. You need the key.\n");
    return 0;
}
