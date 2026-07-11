// /domains/tolkeen/areas/tolkeen_component_alley.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Component Alley");
    set_long(
        "A side alley of component sellers. Jars of powdered bone, copper\n"
        "wire, and bottled PPE-reactive dust. Prices rise when the siege\n"
        "tightens.\n\n"
        "The ward market is west.");
    set_exits( ([
        "west" : "/domains/tolkeen/areas/tolkeen_market"
    ]) );
    set_listen("default", "Glass clink and whispered deals.");
    set_smell("default", "Strange spices and ozone.");
}

void reset() {
    ::reset();
    if(!present("techno-wizard vendor", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_tw_vendor")->move(this_object());
}
