// /d/Praxis/areas/splynn/preserve_reeds.c
// Splynn Preserves hunting loop, R13: Reed Shallows, a spur off the water hole.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Reed Shallows");
    set_long(
        "Tall alien reeds crowd the shallow eastern bank of the water hole,\n"
        "thick enough to swallow the sound of anything moving through them.\n"
        "The mud underfoot is soft and sucking, laced with more of the same\n"
        "deep clawed prints found on the trail. Insects too large for Earth\n"
        "hang in the humid air in slow, droning clouds.\n\n"
        "The open water and the rest of the hunting ground lie back west,\n"
        "through the last stand of reeds.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/preserve_waterhole"
    ]) );
    set_listen("default",
        "The drone of oversized insects, and reeds parting somewhere close\n"
        "by, with nothing visible moving them.");
    set_smell("default",
        "Wet mud, crushed reed stalks, and a faint sweetness that draws the\n"
        "insects in.");
    set_items( ([
        "reeds"    : "Tall alien reeds, dense enough to hide a body a few\n"
                     "steps away. Good cover, in either direction.",
        "mud"      : "Soft, sucking mud that holds tracks well. Several\n"
                     "different things have walked through here recently.",
        "insects"  : "Fat, droning insects, each the size of a fist, drifting\n"
                     "in slow clouds over the wet ground."
    ]) );
}

void reset() {
    ::reset();
    if(random(10) < 5 && !present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
    if(random(10) < 4 && !present("xiticix warrior", this_object()))
        clone_object(MONSTER_PATH+"xiticix_warrior.c")->move(this_object());
}
