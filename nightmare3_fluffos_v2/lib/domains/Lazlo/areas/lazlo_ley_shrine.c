// /domains/Lazlo/areas/lazlo_ley_shrine.c
// A nexus point where ley lines converge.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("ley_line_room", 1);
    set_property("is_ley_line", 1);
    set_property("nexus_point", 1);
    set_short("Lazlo, Ley Line Shrine");
    set_long(
        "Three ley lines converge here, visible as rivers of blue-white\n"
        "light that run through the stone floor and up the carved pillars\n"
        "surrounding the shrine. The air is thick with PPE. Even those\n"
        "with no magical ability can feel the pressure of it.\n\n"
        "Stone benches arranged in a circle face a central nexus point\n"
        "where the ley line energies meet. Candles that never go out\n"
        "line the perimeter. The carved pillars show no maker's marks.\n"
        "They predate Lazlo.\n\n"
        "Type 'meditate' to draw from the ley line energy.\n"
        "The magic quarter is south.");
    set_exits( ([
        "south" : "/domains/Lazlo/areas/lazlo_magic_quarter"
    ]) );
    set_listen("default",
        "A sustained hum, like a chord played too low to fully hear. "
        "It resonates in the chest.");
    set_smell("default",
        "Clean ozone and something older. Stone and deep earth and open sky, "
        "all at once.");
    set_items( ([
        "pillars" : "Carved stone pillars with ley line energy running through them. "
                     "The carvings are not in any known script.",
        "nexus" : "The convergence point. Three ley lines meeting in one spot. "
                     "Standing near it is like standing next to a generator.",
        "candles" : "Candles on stone ledges. The flames are steady. "
                     "They have been burning a long time.",
        "benches" : "Stone benches worn smooth by use. "
                     "People come here to meditate and draw from the ley lines.",
        "energy" : "Visible threads of blue-white ley line energy running "
                     "through the floor and pillars."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ley line priest", this_object()))
        clone_object("/domains/Lazlo/npcs/lazlo_ley_priest.c")->move(this_object());
}
