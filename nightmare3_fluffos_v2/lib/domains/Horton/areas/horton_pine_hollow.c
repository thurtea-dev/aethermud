/* /domains/Horton/areas/horton_pine_hollow.c
   Pine hollow north of the farmstead. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Pine Hollow");
    set_long(
        "A bowl of pine and soft needles. The air is cooler here. Game\n"
        "trails cross the hollow floor. North, the ground rises toward an\n"
        "old quarry rim.\n\n"
        "Farmstead south. Wrecked convoy southwest. Hunting blind east.\n"
        "Quarry rim north.");
    set_exits( ([
        "south"     : "/domains/Horton/areas/horton_abandoned_farmstead",
        "southwest" : "/domains/Horton/areas/horton_wrecked_convoy",
        "east"      : "/domains/Horton/areas/horton_hunting_blind",
        "north"     : "/domains/Horton/areas/horton_quarry_rim"
    ]) );
    set_listen("default", "Wind in pines. A distant crack of branches.");
    set_smell("default", "Resin and clean dirt.");
    set_items( ([
        "needles" : "Thick carpet. Soft underfoot.",
        "trails"  : "Deer, boar, and boot prints mixed.",
        "pines"   : "Straight and tall. Not Rifts-mutated - yet."
    ]) );
}

void reset() {
    ::reset();
    if(random(2) == 0 && !present("plains wolf", this_object()))
        clone_object("/domains/Praxis/monsters/plains_wolf")->move(this_object());
}
