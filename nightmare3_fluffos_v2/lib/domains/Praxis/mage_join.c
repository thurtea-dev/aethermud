#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Hollow willow cavity");
    set_long(
        "The inside of the willow trunk is charred hollow space, larger than\n"
        "the tree should allow. Ley-line burn marks cover the walls. A stair\n"
        "descends into a buried chamber. The opening to West Road is outside.");
    set_exits( ([
        "out" : "/domains/Praxis/west_road3"
    ]) );
}

void init() {
    ::init();
    add_action("go_down", "down");
}

int go_down(string str) {
    this_player()->move("/domains/Praxis/mage_hall");
    write("You descend into the buried chamber below.\n");
    return 1;
}
