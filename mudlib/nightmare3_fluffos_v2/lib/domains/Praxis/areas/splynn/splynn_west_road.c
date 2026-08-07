// /d/Praxis/areas/splynn/splynn_west_road.c
// West road off the south road, leading toward the back alley district.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("West Road, Splynn");
    set_long(
        "A narrower road running west from the south road, less trafficked\n"
        "and notably grimier than the main market area. The buildings here\n"
        "have no signs. The light is worse. The people who do pass through\n"
        "move like they know exactly where they are going and do not want to\n"
        "be asked about it.\n\n"
        "The road ends to the south in a cramped alley district, the kind\n"
        "of place that only exists because the city's designers left it\n"
        "out of their plans intentionally.");
    set_exits( ([
        "east" : "/domains/Praxis/areas/splynn/splynn_south_road",
        "south" : "/domains/Praxis/areas/splynn/splynn_back_alley"
    ]) );
    set_listen("default",
        "Distant market noise, and closer: something metallic being dragged\n"
        "across stone somewhere to the south.");
    set_smell("default",
        "Wet stone, machine oil, and something chemical being vented\n"
        "from one of the unmarked buildings.");
    set_items( ([
        "buildings" : "Unmarked, windowless. Whatever happens here,\n"
                      "nobody wants it advertised.",
        "road" : "Narrower than the main streets, with rougher stone.\n"
                      "The city planners left this district off the official maps.",
        "light" : "Dimmer than the market. The overhead bioluminescent panels\n"
                      "that illuminate the main plaza do not extend here."
    ]) );
}

void reset() {
    ::reset();
}
