// /domains/chitown/areas/cs_residential.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Chi-Town, Residential Block");
    set_long(
        "Identical housing modules in neat rows. Windows show CS decals.\n"
        "A lobby door leads into an apartment block. Near a utility alcove,\n"
        "a service hatch sits flush with the ferrocrete - easy to miss.\n\n"
        "The shopping arcade is southwest. The apartment lobby is east.\n"
        "The hatch leads down.");
    set_exits( ([
        "southwest" : "/domains/chitown/areas/cs_shopping_arcade",
        "east"      : "/domains/chitown/areas/cs_apartment_lobby",
        "hatch"     : "/domains/chitown/areas/cs_service_hatch"
    ]) );
    set_listen("default", "Quiet. Curtains drawn. A distant drone.");
    set_smell("default", "Filtered air.");
    set_items( ([
        "modules" : "Gray housing. Same design on every block.",
        "hatch" : "A service hatch. The bolts look recently worked.",
        "alcove" : "Utility access. Pipes and a locked panel."
    ]) );
}

void reset() {
    ::reset();
    if(!present("cs dog boy", this_object()))
        clone_object("/domains/chitown/npcs/cs_dog_boy")->move(this_object());
}
