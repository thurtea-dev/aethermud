/* /domains/PuertoAngel/areas/pa_harbor.c
   Puerto Angel Harbor -- working docks under vampire management.
   Trade comes through here; so do dangers at night.
   Connects north to the beach. */

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Puerto Angel Harbor");
    set_long(
        "The harbor is functional, which is more than most coastal towns can\n"
        "say. Fishing boats and small cargo vessels ride at anchor, maintained\n"
        "to a standard that suggests whoever runs this port takes pride in it.\n"
        "Nets are mended. Ropes are coiled. Crates are stacked correctly.\n\n"
        "The dockmaster is a secondary vampire who has managed this harbor\n"
        "longer than the town remembers. Trade comes through: salvage, fish,\n"
        "things that do not appear on any manifest. They ask no questions\n"
        "if you ask none.\n\n"
        "At night the harbor changes. The boats come in with cargo that does\n"
        "not move the way cargo should. Wild vampires sometimes follow the\n"
        "boats in from offshore, young ones, undisciplined, not bound by\n"
        "whatever arrangement the town has made with its older residents.\n\n"
        "The beach is north.");
    set_exits( ([
        "north" : "/domains/PuertoAngel/areas/pa_beach"
    ]) );
    set_listen("default",
        "Rigging in the wind. Water against hulls. Distant nets being hauled.\n"
        "At night: splashing that is too deliberate to be the tide.");
    set_smell("default",
        "Salt water, fish, wet rope, and something sweet that has no business\n"
        "being in a working harbor.");
    set_items( ([
        "boats"      : "Fishing boats and small cargo vessels, anchored or docked. "
                       "All maintained. None of them flies any flag.",
        "crates"     : "Stacked along the dock wall. Some labeled, most not. "
                       "They do not all smell like fish.",
        "nets"       : "Fishing nets, mended and hung to dry. The stitching is perfect.",
        "dockmaster" : "The vampire who runs this harbor. Old. Practical. Civil, if approached correctly.",
        "ropes"      : "Coiled with military precision on iron cleats. Someone here has standards.",
        "dock"       : "Old wood, well-maintained. Iron cleats, good rope. "
                       "This harbor has been in continuous use for a long time.",
        "water"      : "The Pacific. At night, shapes move just below the surface near the dock."
    ]) );
}

void reset() {
    object vamp;
    int tod;

    ::reset();
    if(!present("dockmaster", this_object()))
        clone_object("/domains/PuertoAngel/npcs/pa_dockmaster.c")->move(this_object());
    tod = time() % 86400;
    if(tod < 43200) return;

    if(!present("wild vampire", this_object())) {
        vamp = clone_object(MONSTER_PATH + "wild_vampire.c");
        vamp->move(this_object());
    }
    if(random(2) == 0) {
        vamp = clone_object(MONSTER_PATH + "wild_vampire.c");
        vamp->move(this_object());
    }
}
