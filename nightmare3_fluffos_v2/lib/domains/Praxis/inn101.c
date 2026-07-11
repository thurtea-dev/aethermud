// /d/Praxis/inn101.c
// Survivors' Hostel, Common Room. Main entrance from Monument Square.

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/Praxis/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_property("rest_allowed", 1);
    set_short("Survivors' Hostel, Common Room");
    set_long(
        "The front room of the Survivors' Hostel smells of old coffee and\n"
        "gun oil. Mismatched chairs line the walls. A corkboard near the\n"
        "door is thick with handwritten notes, wanted flyers, and maps with\n"
        "routes marked in faded ink. A single working light fixture buzzes\n"
        "overhead, casting everything in flat yellow light.\n\n"
        "The hostel has stood here since before the Rifts. The walls show\n"
        "it. Sal has run this place for as long as anyone can remember.\n\n"
        "Bunk rooms are east through the hallway. Rest is free.");
    set_exits( ([
        "southeast" : "/domains/Praxis/areas/monument_square",
        "east"      : "/domains/Praxis/inn102"
    ]) );
    set_listen("default",
        "The buzz of the overhead light and the sound of the city outside.");
    set_smell("default",
        "Old coffee, machine oil, and the particular smell of a place that "
        "has housed survivors for a very long time.");
    set_items( ([
        "corkboard" : "Covered in notes, wanted flyers, and hand-drawn maps. "
                      "Routes to safe zones, warnings about hostile areas, "
                      "requests for information. The paper is layered thick.",
        "board"     : "See: corkboard",
        "chairs"    : "Mismatched salvage. All functional. Several occupied.",
        "counter"   : "Reinforced steel counter. Sal's workspace.",
        "light"     : "A single working fluorescent tube. Buzzing but holding.",
        "walls"     : "Patched and re-patched over the years. Every patch has a story.",
        "notes"     : "Handwritten. Rations offered for information. "
                      "Warnings about specific roads. Contact ciphers."
    ]) );
}

void reset() {
    ::reset();
    if(!present("sal", this_object()))
        clone_object(NPC_PATH+"sal")->move(this_object());
}
