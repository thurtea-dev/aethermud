/* /domains/Praxis/areas/splynn/alvurron/alvurron_spire_top.c
   Alvurron, Spire Crown. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, Spire Crown");
    set_long(
        "The crown of the spire opens under a dome of black glass through\n"
        "which the rift-light of Splynn wheels slowly. Here the Overlord\n"
        "of Alvurron holds court over the gargoyle city, wings mantled,\n"
        "on a throne of fused bone and stone. Supplicants keep to the\n"
        "edges. The stair descends.");
    set_exits( ([
        "down" : "/domains/Praxis/areas/splynn/alvurron/alvurron_spire"
    ]) );
    set_listen("default",
        "The Overlord's breathing, slow as a bellows.");
    set_smell("default", "Old blood and hot stone.");
    set_items( ([
        "dome" : "Black glass, through which the rift storms of Atlantis
wheel in slow silence.",
        "throne" : "Fused bone and stone. You choose not to look closely at
the bone."
    ]) );
}

void reset() {
    ::reset();
    if(!present("gargoyle overlord", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_overlord_boss")->move(this_object());
}
