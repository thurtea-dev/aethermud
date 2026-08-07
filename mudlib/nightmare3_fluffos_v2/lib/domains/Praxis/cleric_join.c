#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no castle", 1);
    set_short("the old clerics' church of Praxis");
    set_long(
        "The small church stands mostly empty now. Dust covers the pews and\n"
        "the faith that once radiated from this place has gone quiet. The\n"
        "clerics who trained here are long gone, and no one initiates anyone\n"
        "into their old order anymore. A passage north leads into the\n"
        "sanctum they left behind.");
    set_items( ([
        "church"  : "It holds a small chapel west and an empty sanctum for "
            "clerics north.",
        "light"   : "A faint %^BLUE%^blue%^RESET%^ light lingers here, worn thin.",
        "passage" : "It leads north into the cleric's old sanctum."
    ]) );
    set_exits( ([
        "east" : "/domains/Praxis/n_centre1"
    ]) );
}

void init() {
    ::init();
    add_action("go_north", "north");
}

int go_north(string str) {
    this_player()->move("/domains/Praxis/cleric_hall");
    write("You walk north into the cleric's old sanctum.\n");
    return 1;
}
