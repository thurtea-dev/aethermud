// /domains/Praxis/areas/splynn/ocean_rift.c
// The ocean rift off the Splynn coast.
// The Sword of Atlantis spawns here once, ever: UNIQUE_ITEMS_D
// remembers the first player pickup across reboots and reset() stops
// respawning it after that.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("water", 1);
    set_property("rest_allowed", 0);
    set_short("Ocean Rift");
    set_long(
        "The grey Atlantic drops away here into a dimensional rift that opens\n"
        "in the ocean floor. Dimensional energy warps the water into unnatural\n"
        "shapes: spirals and edges where no liquid should hold a form.\n\n"
        "The rift glows faintly blue and gold at its edges, colors that belong\n"
        "to no natural phenomenon. In the depths of the rift, partially visible\n"
        "through the energized water, the outline of something ancient and huge\n"
        "suggests an Atlantean ruin far below the surface.\n\n"
        "A natural stone shelf juts from the rock at the rift's edge, the only\n"
        "stable surface above the drop into the depths.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/splynn_docks",
        "down"  : "/domains/Praxis/areas/splynn/ocean/ocean_rift_shallows"
    ]) );
    set_listen("default",
        "The low resonant hum of dimensional energy and the muffled roar of\n"
        "the Atlantic pressing against something that should not be there.");
    set_smell("default",
        "Salt water, ozone from the dimensional energy, and something ancient\n"
        "that predates the Rifts by thousands of years.");
    set_items( ([
        "rift"   : "A dimensional rift opening in the ocean floor. The edges pulse\n"
                   "with blue and gold energy. Looking directly into it is unwise.",
        "water"  : "The Atlantic, warped by dimensional energies into shapes it\n"
                   "should not hold. The water is cold and dense with salt.",
        "ruin"   : "Visible deep below through the rift: Atlantean stonework.\n"
                   "Architecture from before the Rifts, or during, or after.\n"
                   "Time does not work normally near dimensional openings.",
        "shelf"  : "A natural stone shelf at the rift's edge. The only stable\n"
                   "surface. It looks like a place where something was once\n"
                   "set down deliberately.",
        "glow"   : "Blue and gold light from the rift edges, colors that\n"
                   "belong to whatever opened this tear in the ocean floor."
    ]) );
}

void reset() {
    int taken;

    ::reset();
    taken = 0;
    catch(taken = (int)UNIQUE_ITEMS_D->query_taken("sword_of_atlantis"));
    if(!taken && !present("sword of atlantis", this_object()))
        clone_object(EQ_PATH + "sword_of_atlantis.c")->move(this_object());
    if(!present("sea serpent", this_object()))
        clone_object("/domains/Praxis/monsters/sea_serpent")->move(this_object());
    if(!present("atlantean guardian", this_object()) && random(2))
        clone_object("/domains/Praxis/monsters/atlantean_guardian")->move(this_object());
}
