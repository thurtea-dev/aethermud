#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_short("North end of West Road");
    set_long(
        "West Road ends where the forest swallows the pavement. A massive\n"
        "willow tree leans over the lane, its roots split by ley-line scarring.\n"
        "A hollow at the base might once have been a hideout. It smells of\n"
        "ozone and old ash. Boc La Road lies far south.");
    set_items( ([
        "tree" : "An ancient willow scarred by dimensional energy. The trunk is hollow.",
        "hole" : "A collapsed cavity under the roots. Nothing useful remains inside.",
        "road" : "Cracked asphalt shaded by the tree."
    ]) );
    set_exits( ([
        "south" : "/domains/Praxis/west_road2"
    ]) );
}

void init() {
    ::init();
    add_action("enter_hollow", "enter");
    add_action("enter_hollow", "search");
}

int enter_hollow(string str) {
    if(!str) return 0;
    if(strsrch(lower_case(str), "hole") == -1 &&
       strsrch(lower_case(str), "tree") == -1 &&
       strsrch(lower_case(str), "hollow") == -1)
        return 0;
    write("The hollow trunk collapsed long ago. You find only ash and old bones.\n");
    return 1;
}
