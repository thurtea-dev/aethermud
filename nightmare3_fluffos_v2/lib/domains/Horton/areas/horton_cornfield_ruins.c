/* /domains/Horton/areas/horton_cornfield_ruins.c
   Ruined cornfield between farmstead and bandit camp. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Cornfield Ruins");
    set_long(
        "Stalks grow wild in crooked rows, taller than a man. Irrigation\n"
        "ditches are dry cracks. A scarecrow hangs in pieces from a pole.\n"
        "Good place to get lost - or ambushed.\n\n"
        "Abandoned farmstead west. Bandit camp south.");
    set_exits( ([
        "west"  : "/domains/Horton/areas/horton_abandoned_farmstead",
        "south" : "/domains/Horton/areas/horton_bandit_camp"
    ]) );
    set_listen("default", "Stalks rattling. Something moving between rows.");
    set_smell("default", "Dry corn and dust.");
    set_items( ([
        "stalks"    : "Dense enough to hide a truck.",
        "scarecrow" : "Rags and wire. Shot through more than once.",
        "ditches"   : "Empty. Cracked mud at the bottom."
    ]) );
}

void reset() {
    ::reset();
    if(!present("feral boar", this_object()))
        clone_object("/domains/Horton/monsters/feral_boar")->move(this_object());
}
