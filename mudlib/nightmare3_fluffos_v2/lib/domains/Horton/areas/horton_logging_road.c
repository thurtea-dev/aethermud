/* /domains/Horton/areas/horton_logging_road.c
   Old logging road east of the forest edge. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Logging Road");
    set_long(
        "A rutted logging road runs east into thicker timber. Stumps and\n"
        "rusted cable mark where crews worked before the Rifts. The road\n"
        "is still the easiest path if you ignore the feeling of being watched.\n\n"
        "Forest edge west. Ley glade east.");
    set_exits( ([
        "west" : "/domains/Horton/areas/horton_forest_edge",
        "east" : "/domains/Horton/areas/horton_ley_glade"
    ]) );
    set_listen("default", "Crows. Distant chain clink that might be memory.");
    set_smell("default", "Sawdust ghosts and wet bark.");
    set_items( ([
        "stumps" : "Wide as tables. Cut clean, long ago.",
        "cable"  : "Rusted logging cable half-buried in dirt.",
        "ruts"   : "Deep enough to twist an ankle."
    ]) );
}

void reset() {
    ::reset();
    if(random(2) == 0 && !present("plains wolf", this_object()))
        clone_object("/domains/Praxis/monsters/plains_wolf")->move(this_object());
}
