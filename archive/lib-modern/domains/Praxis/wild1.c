#include <std.h>
#include <rooms.h>

#define MON_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("night light", 1);
    set_property("outdoors", 1);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_property("is_wilderness", 1);
    set_short("Wilderness east of Praxis");
    set_long(
        "Scrub and twisted brush thicken as you leave the Praxis road network.\n"
        "Rift-touched vegetation crowds the eastward trail. Bandits and D-Bee\n"
        "predators prowl these margins.");
    set_items( ([
        "brush"      : "Dry scrub mixed with unnaturally vivid green shoots.",
        "vegetation" : "Post-apocalyptic wilderness closing in from the east.",
        "trail"      : "A faint path toward deeper wild lands."
    ]) );
    set_exits( ([
        "east"  : "/domains/Praxis/wild2",
        "west"  : "/domains/Praxis/e_boc_la3"
    ]) );
}

void reset() {
    int i;

    ::reset();
    if(!present("brodkil demon", this_object())) {
        i = 1 + random(2);
        while(i-- > 0)
            clone_object(MON_PATH + "brodkil_demon.c")->move(this_object());
    }
    if(!present("wild vampire", this_object()) && !random(3))
        clone_object(MON_PATH + "wild_vampire.c")->move(this_object());
}
