// /domains/chitown/areas/cs_apartment_lobby.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Chi-Town, Apartment Lobby");
    set_long(
        "A narrow lobby with mail slots and a directory of unit numbers.\n"
        "The elevator is out of order. Stairs only. A propaganda screen\n"
        "loops silent victory footage.\n\n"
        "The residential street is west.");
    set_exits( ([
        "west" : "/domains/chitown/areas/cs_residential"
    ]) );
    set_listen("default", "A ventilation fan. Footsteps upstairs.");
    set_smell("default", "Floor wax and recycled air.");
    set_items( ([
        "directory" : "Names and unit numbers. All human. All approved.",
        "screen" : "Silent CS victory footage on loop."
    ]) );
}

void reset() {
    ::reset();
    if(!present("chi-town citizen", this_object()))
        clone_object("/domains/chitown/npcs/cs_citizen")->move(this_object());
}
