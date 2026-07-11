/* /d/Praxis/areas/splynn/splynn_forest_edge.c
   Forest edge east of the Splynn Preserves.
   Something enormous is buried in the clearing floor.
   Shovel spawns here. Dig sends player into the buried chamber.
   The EX-5 can move through this room (ex5_accessible). */

#include <std.h>
#include <rooms.h>

#define SHOVEL_PATH "/domains/Praxis/equipment/shovel.c"
#define SPLYNN_UNDER "/domains/Praxis/areas/splynn/splynn_underground"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 1);
    set_property("ex5_accessible", 1);
    set_short("Splynn Forest Clearing");
    set_long(
        "A rough clearing at the eastern edge of the Preserves where the\n"
        "alien undergrowth has been pushed aside by ground heave. The soil\n"
        "here is loose and disturbed, as if something enormous shifted\n"
        "beneath it decades ago and was never fully settled.\n\n"
        "The ground in the center is soft. Darker soil shows through the\n"
        "grass in a rough rectangle the size of a building. When you step\n"
        "close the floor seems to flex under your feet, hollow below.\n\n"
        "A military entrenching shovel is stuck in the ground near the edge.\n"
        "Someone left it here and never came back for it.\n\n"
        "Exits: west into the Preserves.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/splynn_preserves"
    ]) );
    set_listen("default",
        "The alien bird-calls of the Preserves, and a faint metallic\n"
        "echo beneath your feet when you scuff the ground.");
    set_smell("default",
        "Damp alien soil, vegetation, and underneath it all,\n"
        "the ghost of machine oil.");
    set_items( ([
        "ground" : "The central patch of dark soil. It is softer than the\n"
                   "surrounding area, and there is a hollow sound when you\n"
                   "tap it. Something large is below here. Type 'dig' to open it.",
        "soil"   : "Loose dark soil covering the hollow ground. Type 'dig' to\n"
                   "excavate it. You will need a wielded shovel.",
        "patch"  : "A rectangular patch of disturbed ground, dark against\n"
                   "the surrounding pale alien grass. The outline is too regular\n"
                   "to be natural.",
        "shovel" : "An entrenching shovel stuck upright in the ground at the edge\n"
                   "of the clearing. Heavy military grade. Type 'get shovel' to take it."
    ]) );
}

void reset() {
    ::reset();
    if(!present("shovel", this_object()))
        clone_object(SHOVEL_PATH)->move(this_object());
}
