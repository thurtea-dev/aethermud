/* /domains/PuertoAngel/areas/pa_beach.c
   Puerto Angel Beach -- peaceful, if you watch your back. Rest allowed. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("rest_allowed", 1);
    set_short("Puerto Angel Beach");
    set_long(
        "White sand and Pacific waves. The kind of beach that in the old world\n"
        "would have been crowded with tourists. Now it is empty in daylight and\n"
        "you would rather not think about what it looks like after dark.\n\n"
        "The water is clear and cool. The horizon is clean. The town is behind\n"
        "you to the west, quiet, sealed, watching. A good place to rest, if\n"
        "you watch your back.");
    set_exits( ([
        "west"  : "/domains/PuertoAngel/areas/pa_outskirts",
        "south" : "/domains/PuertoAngel/areas/pa_harbor"
    ]) );
    set_listen("default",
        "Waves. Seabirds. The rhythmic sound of the Pacific. Nothing from town.");
    set_smell("default",
        "Salt, warm sand, and clean ocean air. The smell of the old world.");
    set_items( ([
        "sand"    : "White and clean. Raked, you notice. Someone maintains this beach.",
        "waves"   : "The Pacific. Unchanged by the Rifts, more or less.",
        "horizon" : "Clear water meeting clear sky. A single boat far out.",
        "town"    : "To the west. You can feel it watching even with the windows shut.",
        "water"   : "Clear and cool. Safe to drink if you boil it."
    ]) );
}

void reset() { ::reset(); }
