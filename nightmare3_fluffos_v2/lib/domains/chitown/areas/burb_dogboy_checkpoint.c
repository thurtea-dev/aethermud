// /domains/chitown/areas/burb_dogboy_checkpoint.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Dog Boy Checkpoint");
    set_long(
        "A portable barrier and a Coalition banner mark a side approach\n"
        "toward the main gate road. Sensors on a pole sweep the lane.\n"
        "Humans with papers pass. Everyone else gets sniffed.\n\n"
        "Southwest returns to preacher's square. North joins the main burbs\n"
        "street near the gate.");
    set_exits( ([
        "southwest" : "/domains/chitown/areas/burb_preacher_square",
        "north"     : "/domains/chitown/areas/chitown_burbs"
    ]) );
    set_listen("default",
        "A Dog Boy's quiet pant, the hum of a sensor pole.");
    set_smell("default",
        "Ozone from the sensors and the wet-dog scent of mutant fur.");
    set_items( ([
        "barrier" : "Plastic and metal, stamped CS PROPERTY.",
        "banner" : "HUMANITY FIRST. STRENGTH THROUGH UNITY.",
        "sensors" : "They track heat and movement. Probably more."
    ]) );
}

void reset() {
    ::reset();
    if(!present("dog boy patrol", this_object()))
        clone_object("/domains/chitown/monsters/burb_dog_boy")->move(this_object());
}
