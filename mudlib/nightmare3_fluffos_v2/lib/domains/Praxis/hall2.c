#include <std.h>
#include <rooms.h>

inherit ROOM;

int room_103();
int room_104();

void create() {
    ::create();
    set_properties( ([ "light" : 1, "no castle" : 1 ]) );
    set_short("Praxis Refuge Inn, upper hallway");
    set_long(
        "A quiet corridor on the upper floor of the Praxis Refuge Inn.\n"
        "Numbered guest doors line both walls. Battery lanterns cast a\n"
        "steady glow on worn burgundy carpet.");
    set_items( ([
        "hallway" : "A narrow inn corridor with numbered guest rooms.",
        "carpet"  : "Deep burgundy, worn at the edges.",
        "lanterns": "Battery-powered lanterns bolted to the walls.",
        "doors"   : "Reinforced doors with electronic locks."
    ]) );
    set_exits( ([
        "north" : "/domains/Praxis/hall3",
        "south" : "/domains/Praxis/hall",
        "east"  : ({ "/domains/Praxis/hotelroom103", (: room_103 :) }),
        "west"  : ({ "/domains/Praxis/hotelroom104", (: room_104 :) })
    ]) );
}

int room_103() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(103), this_player()))
        return 1;
    write("The door to room 103 is locked. You need the key.\n");
    return 0;
}

int room_104() {
    if(present((string)"/domains/Praxis/hotel"->query_key_id(104), this_player()))
        return 1;
    write("The door to room 104 is locked. You need the key.\n");
    return 0;
}
