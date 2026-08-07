// /d/Praxis/areas/splynn/preserve_bonefield.c
// Splynn Preserves hunting loop, R4: the bone field, a hub with three ways out.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Bone Field");
    set_long(
        "The undergrowth gives way to an open clearing floored in bone. Ribs\n"
        "and skulls and long shattered limbs lie bleached across the whole\n"
        "expanse, the picked-clean leavings of a hunt that has run here for\n"
        "longer than the city has stood. Nothing grows in the field itself.\n"
        "There is no cover, and that is the point: prey driven into the open\n"
        "has nowhere left to hide.\n\n"
        "Tracks cross the field in every direction. The water hole lies\n"
        "north, a raised hunting blind stands east, and a narrow gully cuts\n"
        "away south into shadow.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/preserve_waterhole",
        "east"  : "/domains/Praxis/areas/splynn/preserve_blind",
        "south" : "/domains/Praxis/areas/splynn/preserve_hollow"
    ]) );
    set_listen("default",
        "The dry click of bone shifting underfoot, and the distant call of\n"
        "something circling high overhead, waiting for the field to be still.");
    set_smell("default",
        "Old dry bone, sun-bleached and scentless, over a faint sweetness\n"
        "from the freshest kills at the field's edge.");
    set_items( ([
        "bone"   : "A carpet of bleached bone: ribs, skulls, limbs, shattered\n"
                   "and scattered. Few of the skulls match any species you know.",
        "skulls" : "Long-jawed, wrong-angled skulls from a dozen worlds. This\n"
                   "field has fed on prey rifted in from everywhere.",
        "tracks" : "Trails crossing the open ground in every direction, worn by\n"
                   "hunters herding their prey into the killing clearing."
    ]) );
}

void reset() {
    ::reset();
    if(!present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
    if(random(10) < 4 && !present("xiticix warrior", this_object()))
        clone_object(MONSTER_PATH+"xiticix_warrior.c")->move(this_object());
}
