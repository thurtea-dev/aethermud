// /domains/chitown/areas/cs_samas_hangar.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Chi-Town, SAMAS Hangar Pad");
    set_long(
        "A reinforced hangar bay. A SAMAS power armor unit stands locked in\n"
        "a charging cradle, wings folded, skull-faced helmet dark. Warning\n"
        "stripes mark the pad. Unauthorized approach is not advised.\n\n"
        "The ISS checkpoint is west. Dog Boy kennels are north.");
    set_exits( ([
        "west"  : "/domains/chitown/areas/cs_iss_checkpoint",
        "north" : "/domains/chitown/areas/cs_dogboy_kennels"
    ]) );
    set_listen("default", "Charging hum and hydraulic ticks from the cradle.");
    set_smell("default", "Ozone, coolant, and machine oil.");
    set_items( ([
        "samas" : "PA-06A SAMAS in a locked cradle. Not for civilians.",
        "cradle" : "Charging clamps and status lights. Green means ready."
    ]) );
}

void reset() {
    ::reset();
    if(!present("samas pilot", this_object()))
        clone_object("/domains/chitown/npcs/cs_samas_pilot")->move(this_object());
}
