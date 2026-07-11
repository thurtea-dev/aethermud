// /domains/Lazlo/lazlo_welcome.c
// Rift arrival point at the City of Lazlo, Ontario.
// Players who choose Lazlo as their starting region arrive here.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Lazlo, Arrival Courtyard");
    set_long(
        "A broad stone courtyard inside the city walls of Lazlo. The rift\n"
        "that brought you here collapses behind you with a sound like a door\n"
        "slamming in a wind. Blue-white ley line energy traces the seams\n"
        "of the flagstones underfoot.\n\n"
        "This is not the Coalition's world. Magic is open here. You can\n"
        "feel it in the air, a constant subtle pressure behind your eyes.\n"
        "Mages in open robes walk past without concealment. D-Bees of a\n"
        "dozen species share the courtyard without incident.\n\n"
        "A directory board on the north wall shows the layout of the city.\n"
        "The main gate opens east, toward the lake. The market quarter is\n"
        "to the west. The library complex is to the north.");
    set_exits( ([
        "north" : "/domains/Lazlo/areas/lazlo_library",
        "east" : "/domains/Lazlo/areas/lazlo_gate",
        "west" : "/domains/Lazlo/areas/lazlo_market"
    ]) );
    set_listen("default",
        "Voices in a dozen languages, the hum of active ley line energy, and somewhere "
        "the sound of someone teaching aloud.");
    set_smell("default",
        "Clean air off the lake, and the faint sharp smell of focused magical energy.");
    set_items( ([
        "courtyard" : "Stone-paved, well-maintained. Nothing here looks improvised.",
        "ley line" : "The energy traces visible in the flagstones. You can feel it if you stand still.",
        "directory" :
            "A board showing the city layout:\n"
            " North - Great Library complex\n"
            " East - Main city gate, Lake of the Gifted beyond\n"
            " West - Market quarter\n"
            " (This courtyard: Rift Arrival Zone)",
        "board" : "See: directory",
        "mages" : "Scholars and practitioners moving freely. Several acknowledge you with a nod.",
        "d-bees" : "At least four species visible from here. Nobody is staring at anyone."
    ]) );
}

void reset() {
    ::reset();
}
