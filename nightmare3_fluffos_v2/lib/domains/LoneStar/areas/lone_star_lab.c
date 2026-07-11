// /domains/LoneStar/areas/lone_star_lab.c
// Lone Star genetics research lab -- restricted interior area.

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 4);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Genetics Research Lab");
    set_long(
        "The research lab is a large open space divided by glass partitions into\n"
        "work bays.  Each bay contains equipment that mixes pre-Rifts clinical\n"
        "hardware with what can only be alien technology recovered from dimensional\n"
        "incidents.  Transparent tanks line the far wall.  Their contents are not\n"
        "immediately identifiable.\n\n"
        "The floor is clean to an almost unsettling degree.  Scientists work in\n"
        "silence behind the glass.  A Coalition guard is stationed at the lab door,\n"
        "watching all movement.  A reinforced corridor leads north to containment.");
    set_exits( ([
        "south" : "/domains/LoneStar/areas/lone_star_plaza",
        "north" : "/domains/LoneStar/areas/lone_star_containment"
    ]) );
    set_listen("default",
        "The low hum of environmental control systems.  The occasional soft electronic chime from equipment.");
    set_smell("default",
        "Strong antiseptic.  Something underneath it that does not have a familiar name.");
    set_items( ([
        "tanks"      : "Large transparent tanks along the wall.  The fluid inside is opaque and pale.  Movement inside the tanks is not visible from here.",
        "equipment"  : "A mix of pre-Rifts clinical hardware and technology that does not match any known manufacturer.",
        "glass"      : "Floor-to-ceiling glass partitions dividing the lab into bays.  Heavy, sealed.",
        "partitions" : "Glass dividers between work areas.  Scientists work on both sides without looking up.",
        "scientists" : "Researchers absorbed in work behind the glass.  None of them look up.",
        "door"       : "A reinforced lab door with a keypad lock.  The guard at the entrance is watching you.",
        "guard"      : "A Coalition Dog Boy soldier.  Eyes on you."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Dog Boy guard", this_object()))
        clone_object(NPC_PATH + "cs_dog_boy_guard.c")->move(this_object());
    if(!present("Coalition scientist", this_object()))
        clone_object(NPC_PATH + "coalition_scientist.c")->move(this_object());
}
