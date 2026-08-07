/* /domains/Praxis/areas/splynn/alvurron/alvurron_deep_rift.c
   Alvurron, the Deep Rift. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Alvurron, the Deep Rift");
    set_long(
        "Beneath the rift chamber the stone gives way to a raw ley-line\n"
        "cavern. A crack in reality hangs above a floor of fused glass,\n"
        "bleeding pale light and half-formed shapes that dissolve before\n"
        "they finish arriving. PPE saturates the air until your teeth\n"
        "ache. The chamber above is up.");
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/alvurron/alvurron_rift_chamber"
    ]) );
    set_listen("default",
        "A sound like wind through a door that is not there.");
    set_smell("default", "Ozone thick enough to taste.");
    set_items( ([
        "crack" : "A wound in the air, edges shivering. Things press against
it from the other side.",
        "glass" : "The floor fused to glass by centuries of rift discharge.",
        "shapes" : "Half-arrived things. They dissolve if you look directly."
    ]) );
}

void reset() {
    ::reset();
    if(!present("rift wisp", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_rift_wisp")->move(this_object());
}
