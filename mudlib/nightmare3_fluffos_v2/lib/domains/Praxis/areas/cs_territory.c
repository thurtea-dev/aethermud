// /d/Praxis/areas/cs_territory.c
// Coalition territory beyond the Praxis checkpoint: stub room.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Coalition Territory, south approach");
    set_long(
        "Beyond the checkpoint the landscape opens into a bleak patrol zone.\n"
        "Razor wire and sensor posts mark a perimeter that stretches in both\n"
        "directions. Coalition hover-drones make slow passes overhead. The\n"
        "distant skyline shows the blocky silhouettes of CS fortifications.\n\n"
        "You have the feeling this is not a place to linger. The checkpoint\n"
        "is back to the north.");
    set_exits( (["north" : "/domains/Praxis/areas/coalition_checkpoint"]) );
    set_listen("default",
        "The low hum of patrol drones and the distant mechanical thud of heavy vehicles.");
    set_smell("default",
        "Exhaust, ozone from charged fencing, and the sharp reek of Coalition chemicals.");
    set_items( ([
        "wire" : "Coils of electrified razor wire line the perimeter.",
        "sensor" : "Motion sensors on short posts, blinking slowly.",
        "drone" : "A boxy Coalition surveillance drone circling overhead.",
        "skyline" : "CS fortifications. Not inviting.",
        "perimeter" : "The boundary of Coalition-controlled territory. Well-defended."
    ]) );
}

void init() {
    ::init();
    if(!this_player() || this_player() == this_object()) return;
    if(this_player()->query_ghost()) return;
    if((int)WANTED_D->is_cs_enemy(this_player()))
        write("A patrol drone tags you as hostile. Coalition forces are watching.\n");
}

void reset() {
    ::reset();
}
