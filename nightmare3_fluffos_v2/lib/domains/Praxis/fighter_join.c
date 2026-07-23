#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no castle", 1);
    set_short("the old Hall of Fighters");
    set_long(
        "The tapestries here have faded and torn, the glory they once\n"
        "depicted barely visible under decades of dust. The fighters who\n"
        "gathered here to train young warriors are long gone, and the\n"
        "staircase up through the ceiling stands quiet now.");
    set_items( ([
        ({"tapestry", "tapestries", "wall"}) : "Faded, torn, and half fallen "
            "from the wall. Whatever scene they once depicted is barely "
            "visible now.",
        ({"hole", "light", "opening", "ceiling"}) : "Up in the ceiling, the "
            "staircase leads through a dim light up to the second floor.",
        "staircase" : "It leads through the opening in the ceiling to the "
            "second floor."
    ]) );
    set_exits( ([
        "southeast" : "/domains/Praxis/s_centre4"
    ]) );
}

void init() {
    ::init();
    add_action("go_up", "up");
}

int go_up(string str) {
    this_player()->move("/domains/Praxis/fighter_hall");
    write("You climb the spiraling staircase up to the second floor.\n");
    return 1;
}
