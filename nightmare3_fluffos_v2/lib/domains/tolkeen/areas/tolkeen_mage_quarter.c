// /domains/tolkeen/areas/tolkeen_mage_quarter.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_property("no attack", 1);
    set_short("Tolkeen, Mage Quarter");
    set_long(
        "Narrow streets of workshops and lecture halls. Floating lamps.\n"
        "Apprentices argue over diagrams chalked on ferrocrete.\n\n"
        "Liberation Square is southwest. A techno-wizard workshop is east.\n"
        "The library is north. A healer's clinic is west.");
    set_exits( ([
        "southwest" : "/domains/tolkeen/areas/tolkeen_square",
        "east"      : "/domains/tolkeen/areas/tolkeen_tw_workshop",
        "north"     : "/domains/tolkeen/areas/tolkeen_library",
        "west"      : "/domains/tolkeen/areas/tolkeen_clinic"
    ]) );
    set_listen("default", "Debate, chalk on stone, a distant spell pop.");
    set_smell("default", "Ink, incense, and hot copper.");
}

void reset() {
    ::reset();
    if(!present("tolkeen mage", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_mage")->move(this_object());
}
