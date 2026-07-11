// /domains/tolkeen/areas/tolkeen_inn.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Tolkeen, Lightning Fist Inn");
    set_long(
        "A crowded common room. Mercenaries drink. Mages argue. A board by\n"
        "the door lists wall-duty pay rates.\n\n"
        "Liberation Square is southeast. Guest rooms are upstairs.");
    set_exits( ([
        "southeast" : "/domains/tolkeen/areas/tolkeen_square",
        "up"        : "/domains/tolkeen/areas/tolkeen_inn_rooms"
    ]) );
    set_listen("default", "Low talk and a badly tuned radio.");
    set_smell("default", "Stew and spilled beer.");
}

void reset() {
    ::reset();
    if(!present("tolkeen innkeeper", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_innkeeper")->move(this_object());
}
