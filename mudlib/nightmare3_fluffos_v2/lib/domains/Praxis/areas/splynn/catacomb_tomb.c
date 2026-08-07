// /d/Praxis/areas/splynn/catacomb_tomb.c
// Deepest catacomb chamber. Catacomb wight resides here.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("The Deep Tomb");
    set_long(
        "The tunnel ends in a sealed tomb chamber. The stone here is\n"
        "different: darker, colder, carved with deep channels that run in\n"
        "patterns too regular to be decorative. A sarcophagus of black\n"
        "stone sits against the far wall, its lid displaced by several\n"
        "centimeters.\n\n"
        "The lid has been displaced from the inside.\n\n"
        "Something stands in the corner. It was human once. The cold\n"
        "it radiates is not temperature. It is the absence of life\n"
        "pressed into a shape that still remembers standing upright.\n"
        "Its eyes are points of grey light in a face that no longer\n"
        "moves the way faces should.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/catacomb_chamber"
    ]) );
    set_listen("default",
        "Nothing. The dead do not breathe. The silence here is\n"
        "complete and specific.");
    set_smell("default",
        "Cold. Mineral. Old death without decay: preserved by something\n"
        "that does not require the usual processes.");
    set_items( ([
        "sarcophagus" : "Black stone, massive. The lid has been displaced from the\n"
                        "inside. The interior is empty. Whatever was inside it\n"
                        "is now standing in the corner.",
        "channels" : "Deep channels cut into the stone floor and walls, arranged\n"
                        "in complex interlocking patterns. Containment. Or invitation.\n"
                        "At this point the distinction may not matter.",
        "lid" : "Stone sarcophagus lid, shifted several centimeters. The displacement\n"
                        "required considerable force. From the inside.",
        "corner" : "Something stands in the corner. It has noticed you.\n"
                        "It has been waiting for someone to come down here for a long time.",
        "eyes" : "Points of grey light. They track your movement.\n"
                        "There is intent behind them. The intent is not friendly."
    ]) );
}

void reset() {
    ::reset();
    if(!present("catacomb wight", this_object()))
        clone_object(MONSTER_PATH+"catacomb_wight.c")->move(this_object());
}
