// /d/Praxis/areas/splynn/preserve_saddle.c
// Splynn Preserves hunting loop, R20: Wind-Scoured Saddle, closes the loop
// between High Nest and the Overgrown Ridge.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Wind-Scoured Saddle");
    set_long(
        "A narrow spine of bare rock links two high places: the nest's\n"
        "rocky bowl to the east, and the grass-crowned ridge to the south.\n"
        "Both sit far above the low ground where most of the hunting takes\n"
        "place, and something learned long ago to walk the exposed spine\n"
        "between them rather than climb all the way back down.\n\n"
        "The wind never stops moving through here. Loose scree makes every\n"
        "step a small risk, but the shortcut saves a long walk through the\n"
        "lower ground.");
    set_exits( ([
        "east"  : "/domains/Praxis/areas/splynn/preserve_nest",
        "south" : "/domains/Praxis/areas/splynn/preserve_ridge"
    ]) );
    set_listen("default",
        "Wind scouring bare stone from both directions at once, and the\n"
        "occasional clatter of loose scree finding its own way down.");
    set_smell("default",
        "Clean, thin air, and dry rock dust.");
    set_items( ([
        "spine" : "A narrow spine of bare rock, wind-scoured smooth in\n"
                  "places and treacherous with loose scree in others.",
        "scree" : "Loose broken stone that shifts underfoot. Watch your\n"
                  "footing."
    ]) );
}

void reset() {
    ::reset();
    if(random(10) < 3 && !present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
}
