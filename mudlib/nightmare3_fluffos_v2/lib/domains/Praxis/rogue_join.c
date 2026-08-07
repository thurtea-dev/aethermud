#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Abandoned rogue safehouse");
    set_long(
        "What looks like a worn shack opens into reinforced walls and iron\n"
        "bracing. The old pre-Rifts rogue guild abandoned this outlands nest\n"
        "after the Rifts. Stairs descend to their old war room.");
    set_exits( ([
        "out" : "/domains/Jungle/Outlands/hideout"
    ]) );
}

void init() {
    ::init();
    add_action("go_down", "down");
}

int go_down(string str) {
    this_player()->move("/domains/Praxis/rogue_hall");
    write("You descend into the rogue war room.\n");
    return 1;
}
