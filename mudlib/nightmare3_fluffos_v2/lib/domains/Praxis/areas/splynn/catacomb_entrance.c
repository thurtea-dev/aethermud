// /d/Praxis/areas/splynn/catacomb_entrance.c
// Catacomb entrance below the back alley grate.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Catacomb Entrance");
    set_long(
        "A rough-cut shaft drops down from the street above, ending in a\n"
        "low stone chamber. Someone has placed an iron ladder against the\n"
        "shaft wall, old but serviceable. The ceiling here is carved stone,\n"
        "older than anything above ground in Splynn: pre-Rifts, pre-Splugorth.\n\n"
        "A tunnel runs south into darkness. The air is cold and dry and\n"
        "carries the smell of old stone and something that might be incense,\n"
        "though nothing has burned here in a very long time.\n\n"
        "Someone carved writing above the tunnel entrance, in a script that\n"
        "predates the Rifts. You cannot read it. It does not feel welcoming.");
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/splynn_back_alley",
        "south" : "/domains/Praxis/areas/splynn/catacomb_hall"
    ]) );
    set_listen("default",
        "Your own breathing. The creak of the iron ladder. From the south,\n"
        "nothing. Complete silence, which is somehow worse.");
    set_smell("default",
        "Cold dry stone and the ghost of something that might have been incense\n"
        "centuries ago.");
    set_items( ([
        "ladder" : "Iron ladder bolted to the shaft wall. Old but solid. Someone\n"
                    "maintains this route, or maintained it once.",
        "shaft" : "The shaft leading back up to the alley. The grate is visible\n"
                    "as a grid of light far above.",
        "writing" : "Pre-Rifts script carved deep into the stone above the tunnel\n"
                    "entrance. Whatever language this is, it predates the Coalition.\n"
                    "It predates most things still standing.",
        "tunnel" : "The tunnel runs south into complete darkness.\n"
                    "Your light source will matter here.",
        "ceiling" : "Carved stone. The work is precise and old. Not Splugorth.\n"
                    "Not anything still commonly practiced."
    ]) );
}

void reset() {
    object npc;
    ::reset();
    if(!present("tattoo master", this_object())) {
        npc = clone_object("/domains/Praxis/npcs/tattoo_master");
        npc->move(this_object());
    }
}
