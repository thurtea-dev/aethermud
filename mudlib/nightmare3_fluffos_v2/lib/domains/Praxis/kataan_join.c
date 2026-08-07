#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no castle", 1);
    set_short("the old cavern of the kataans");
    set_long(
        "Deep, intricate murals and symbols are carved into the walls of\n"
        "this underground cavern just north of Praxis, faded and half\n"
        "crumbled now. Whatever coven once trained here is long gone. A\n"
        "passage leads down into the cavern they left behind.");
    set_items( ([
        "mural"  : "Old and worn, depicting rituals no one remembers anymore.",
        "murals" : "Ancient and crumbling, showing bizarre practices from an "
            "order long since gone.",
        "light"  : "A faint %^BLUE%^blue%^RESET%^ light lingers here, worn thin.",
        "cavern" : "The cavern feels hollow now, whatever it once held long "
            "since departed."
    ]) );
    set_exits( ([
        "up" : "/domains/Praxis/forest3"
    ]) );
}

void init() {
    ::init();
    add_action("go_down", "down");
}

int go_down(string str) {
    this_player()->move("/domains/Praxis/kataan_hall");
    write("You descend into the cavern below.\n");
    return 1;
}
