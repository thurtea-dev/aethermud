// /d/Praxis/inn102.c
// Survivors' Hostel, Hallway. Connects all interior rooms.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_property("rest_allowed", 1);
    set_short("Survivors' Hostel, Hallway");
    set_long(
        "A narrow hallway running through the center of the hostel. Bare\n"
        "concrete walls, a worn linoleum floor, and a string of salvaged\n"
        "lights running along the ceiling. Hand-painted signs point to the\n"
        "bunk rooms, kitchen, and office.\n\n"
        "Bunk Room A is north. Bunk Room B is south.\n"
        "The kitchen is east. The manager's office is up the stairs.");
    set_exits( ([
        "west"  : "/domains/Praxis/inn101",
        "north" : "/domains/Praxis/inn103",
        "south" : "/domains/Praxis/inn104",
        "east"  : "/domains/Praxis/inn105",
        "up"    : "/domains/Praxis/inn106"
    ]) );
    set_listen("default",
        "The hum of the building's electrical system and distant city noise.");
    set_smell("default",
        "Concrete dust and old cooking smells from the kitchen.");
    set_items( ([
        "signs"    : "Hand-painted directional signs. Bunk A, Bunk B, Kitchen, Office.",
        "lights"   : "Salvaged light fixtures on a daisy-chained circuit. Functional.",
        "floor"    : "Worn linoleum, clean. Someone sweeps it regularly.",
        "walls"    : "Bare concrete. No decorations. Functional."
    ]) );
}

void reset() { ::reset(); }
