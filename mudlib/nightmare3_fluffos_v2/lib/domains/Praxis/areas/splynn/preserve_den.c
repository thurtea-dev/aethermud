// /d/Praxis/areas/splynn/preserve_den.c
// Splynn Preserves hunting loop, R16: Thornbrake Den, a dead end off the thicket.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Thornbrake Den");
    set_long(
        "The thorn tunnel opens into a low, matted den where the black\n"
        "spines have been dragged and heaped into a rough nest. Old bones,\n"
        "broken armor plate, and a scatter of chewed alien hide cover the\n"
        "floor. Whatever forces its way through the thicket sleeps here\n"
        "between hunts, when it sleeps at all.\n\n"
        "There is no way forward. The only way out is back east, through\n"
        "the tunnel.");
    set_exits( ([
        "east" : "/domains/Praxis/areas/splynn/preserve_thicket"
    ]) );
    set_listen("default",
        "A low, rhythmic breathing from somewhere close in the dark, and\n"
        "the soft click of claws shifting in a heap of old bone.");
    set_smell("default",
        "Old blood, matted fur, and the thick animal reek of a den that has\n"
        "never been aired out.");
    set_items( ([
        "nest"  : "A rough heap of dragged thorn-spine, matted flat by\n"
                  "repeated use. Something sleeps here.",
        "bones" : "Old bones and broken armor plate scattered across the\n"
                  "floor, picked clean long ago.",
        "hide"  : "A scatter of chewed alien hide, torn from something that\n"
                  "did not survive the thicket."
    ]) );
}

void reset() {
    ::reset();
    if(!present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
}
