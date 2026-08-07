/* /domains/Horton/areas/horton_ancient_grove.c
   Old growth west/north of deep forest and ley glade. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Ancient Grove");
    set_long(
        "Trees here predate the town, maybe the roads. Roots braid into\n"
        "walls. The air is still. Deep forest lies west; the ley glade\n"
        "opens south when the light finds a gap.\n\n"
        "Deep forest west. Ley glade south. Wolf den north.");
    set_exits( ([
        "west"  : "/domains/Horton/areas/horton_forest_deep",
        "south" : "/domains/Horton/areas/horton_ley_glade",
        "north" : "/domains/Horton/areas/horton_wolf_den"
    ]) );
    set_listen("default", "Almost nothing. Then a branch that should not move.");
    set_smell("default", "Deep leaf mold.");
    set_items( ([
        "roots" : "Thick as a man's waist. Climbable if you are desperate.",
        "trees" : "Older than any living memory in Horton.",
        "gap"   : "A thin shaft of light toward the glade."
    ]) );
}

void reset() {
    ::reset();
    if(!present("plains wolf", this_object()))
        clone_object("/domains/Praxis/monsters/plains_wolf")->move(this_object());
}
