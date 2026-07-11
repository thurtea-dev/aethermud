/* /domains/Horton/areas/horton_forest_deep.c
   Deep Forest north of Horton -- aggressive wildlife, something ancient. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Deep Forest");
    set_long(
        "The canopy is so thick that midday feels like dusk. Bioluminescent\n"
        "fungi cling to the roots of enormous trees, providing the only light.\n"
        "Wildlife here is aggressive and large. The ground is soft and silent\n"
        "underfoot in a way that feels deliberate. Something old lives deeper\n"
        "in. The townsfolk disagree on what, but they all agree it is there.");
    set_exits( ([
        "south" : "/domains/Horton/areas/horton_forest_edge"
    ]) );
    set_listen("default",
        "Your own heartbeat. The drip of moisture from the canopy. Something breathing.");
    set_smell("default",
        "Deep earth, rotting wood, and something alive and very large.");
    set_items( ([
        "canopy"  : "So thick the sky is invisible. Only green filtered light gets through.",
        "fungi"   : "Bioluminescent. Blue-green. They pulse very slowly.",
        "roots"   : "The root systems are vast. This forest is very old.",
        "darkness": "It watches back."
    ]) );
}

void reset() {
    object wolf;
    object stalker;
    int count;

    count = 0;
    if(!present("plains wolf", this_object())) {
        wolf = clone_object("/domains/Praxis/monsters/plains_wolf");
        wolf->move(this_object());
        count++;
    }
    if(count < 2 && random(2) == 0 && !present("jungle stalker", this_object())) {
        wolf = clone_object("/domains/Praxis/monsters/plains_wolf");
        wolf->move(this_object());
    }
    if(random(5) == 0 && !present("jungle stalker", this_object())) {
        stalker = clone_object("/domains/Praxis/monsters/jungle_stalker");
        stalker->move(this_object());
    }
    ::reset();
}
