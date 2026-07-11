// /domains/tolkeen/areas/tolkeen_market.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_property("no attack", 1);
    set_short("Tolkeen, Ward Market");
    set_long(
        "Stalls sell spell components, MDC scrap, and Coalition trophies\n"
        "repurposed into tools. A raised platform lets a techno-wizard watch\n"
        "the crowd for hostile magic.\n\n"
        "Liberation Square is west. A component alley opens east. Refugee\n"
        "tents crowd south along the inner wall.");
    set_exits( ([
        "west"  : "/domains/tolkeen/areas/tolkeen_square",
        "east"  : "/domains/tolkeen/areas/tolkeen_component_alley",
        "south" : "/domains/tolkeen/areas/tolkeen_refugee_yard"
    ]) );
    set_listen("default", "Haggling and the crackle of a dampening ward.");
    set_smell("default", "Incense, oil, and hot metal.");
}

void reset() {
    ::reset();
    if(!present("techno-wizard vendor", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_tw_vendor")->move(this_object());
}
