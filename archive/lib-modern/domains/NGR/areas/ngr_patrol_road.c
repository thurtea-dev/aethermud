/* /domains/NGR/areas/ngr_patrol_road.c
   NGR patrol road east of the checkpoint. Active patrol presence.
   Spawns ngr_patrol_soldier on reset. */

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/NGR/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("ngr_territory", 1);
    set_short("NGR Patrol Road");
    set_long(
        "A paved military road cuts through reforested hills, the asphalt\n"
        "maintained in better condition than most of the world can manage.\n"
        "NGR patrol vehicles pass at intervals: armored personnel carriers\n"
        "in grey livery, moving on schedule. The infrastructure here is\n"
        "intact in a way that most of the world has forgotten was possible.\n\n"
        "Guard posts mark the roadside at regular intervals. The treeline\n"
        "to the north is close: Gargoyle territory begins not far past\n"
        "that edge. Patrols here are armed accordingly.\n"
    );
    set_exits( ([
        "west" : "/domains/NGR/areas/ngr_checkpoint",
        "east" : "/domains/NGR/areas/ngr_watchtower"
    ]) );
    set_listen("default",
        "The distant engine of a patrol vehicle. Wind in the reforested hills.\n"
        "A radio check being called in, short and professional.");
    set_smell("default",
        "Fresh asphalt, engine exhaust, and the resin smell of the new-growth\n"
        "forest that NGR reforestation programs planted after the Rifts.");
    set_items( ([
        "road"    : "Maintained military road, asphalt in excellent condition.\n"
                    "The NGR maintains its infrastructure obsessively.",
        "hills"   : "Reforested hills, the trees young and uniform: replanted\n"
                    "by NGR environmental programs. Better than nothing.",
        "posts"   : "Guard posts at roadside intervals: reinforced boxes with\n"
                    "a slit window and a rail gun mount. Currently manned.",
        "vehicles" : "NGR armored personnel carriers, Triax TX-APC-2. Grey.\n"
                     "They move on a schedule and do not stop for pedestrians.",
        "treeline" : "The forest edge to the north. Past it, NGR patrols have\n"
                     "regular contact with Gargoyle raiding parties. The trees\n"
                     "are full of things that do not belong to this dimension."
    ]) );
}

void reset() {
    ::reset();
    if(!present("NGR patrol soldier", this_object()))
        clone_object(NPC_PATH + "ngr_patrol_soldier")->move(this_object());
}
