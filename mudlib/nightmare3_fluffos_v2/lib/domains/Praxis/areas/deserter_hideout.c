// /d/Praxis/areas/deserter_hideout.c
// Hidden room off the back alley: CS deserter's refuge.
// The south exit from back_alley leads here (hidden).

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Concealed Alcove, Behind the Back Alley");
    set_long(
        "A narrow space between two building foundations, covered overhead by\n"
        "a collapsed roof section that turned the gap into an accidental shelter.\n"
        "Someone has been here. A bedroll is wedged against the far wall next\n"
        "to a pack stuffed with CS-issue equipment. Empty ration packs. A\n"
        "canteen. The occupant has not been gone long.\n\n"
        "Crouched in the corner, watching you with wide eyes, is a young man\n"
        "still wearing the lower half of a CS infantry uniform.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/back_alley"
    ]) );
    set_listen("default",
        "Breathing. His. He is trying to be quiet and failing.");
    set_smell("default",
        "Fear sweat and Coalition-issue boot polish. Standard CS footsoldier.");
    set_items( ([
        "bedroll" : "Standard CS field bedroll. Regulation, but the patch on the corner was cut off.",
        "pack" : "CS field pack. Heavy. He took everything he could carry when he ran.",
        "rations" : "Empty CS ration packs. He has been here for at least three days.",
        "canteen" : "CS-issue aluminum canteen. Still has water in it.",
        "uniform" : "Lower half of a CS infantry uniform. He cut the insignia off the top half.",
        "equipment" : "CS-issue field equipment. Standard infantryman loadout, minus the weapon."
    ]) );
}

void reset() {
    if(!present("deserter", this_object()))
        new("/domains/Praxis/monsters/deserter")->move(this_object());
}
