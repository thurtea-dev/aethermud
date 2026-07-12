// /d/Praxis/areas/splynn/preserve_hollow.c
// Splynn Preserves hunting loop, R10: hunted hollow, dead-end branch off R4.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Hunted Hollow");
    set_long(
        "The gully drops into a dead-end hollow, sunk deep enough that the\n"
        "light goes grey and the walls close in on three sides. This is where\n"
        "the driven prey ends up: escaped stock and captured D-bees run down\n"
        "out of the bone field and cornered here with nowhere left to go.\n"
        "Scratch-marks claw the earth walls where the desperate tried to\n"
        "climb out. None of them reached the top.\n\n"
        "The only way out is back north, up the gully to the field. The\n"
        "slavers know it too, and they work this hollow last, at their\n"
        "leisure.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/preserve_bonefield"
    ]) );
    set_listen("default",
        "Something breathing hard in the dark at the back of the hollow, and\n"
        "the scrape of a guard settling in to block the only way out.");
    set_smell("default",
        "Cold earth, old fear-sweat soaked into the ground, and the reptile\n"
        "musk of the slavers who work this trap.");
    set_items( ([
        "walls"    : "Steep earth walls raked top to bottom with claw-marks,\n"
                     "where cornered prey tried and failed to climb free.",
        "scratches" : "Desperate gouges in the packed earth. Whatever made them\n"
                      "did not get out of this hollow alive.",
        "gully"    : "The narrow cut running north back up to the bone field,\n"
                     "the only way in and the only way out."
    ]) );
}

void reset() {
    ::reset();
    if(!present("splugorth minion", this_object()))
        clone_object(MONSTER_PATH+"splugorth_minion.c")->move(this_object());
    if(random(10) < 5 && !present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
}
