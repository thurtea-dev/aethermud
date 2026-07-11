// /domains/demon_plane/areas/demon_sliver_shrine.c
// Loot room: rift shard amulet on a demon altar.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("demon_plane", 1);
    set_property("off_world", 1);
    set_short("Demon-Plane Sliver, Rift Shrine");
    set_long(
        "A basalt altar squats at the center of a circular chamber. Green fire\n"
        "burns without fuel in braziers around the walls. A crystalline amulet\n"
        "rests on the altar, pulsing in time with the ley scar you came through.\n"
        "Whatever left it here wanted the tear to stay open.");
    set_exits( ([
        "south" : "/domains/demon_plane/areas/demon_sliver_corridor"
    ]) );
    set_items( ([
        "altar" : "Black basalt carved with runes that hurt to read. Something gleams on top.",
        "amulet" : "A rift shard amulet. You could take it if you survive the guardian."
    ]) );
}

void reset() {
    ::reset();
    if(!present("demon sliver alpha", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_alpha")->move(this_object());
    if(!present("rift shard amulet", this_object()))
        new("/domains/demon_plane/equipment/rift_shard_amulet")->move(this_object());
}
