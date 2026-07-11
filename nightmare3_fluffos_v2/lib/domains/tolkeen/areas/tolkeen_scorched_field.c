// /domains/tolkeen/areas/tolkeen_scorched_field.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Scorched Field");
    set_long(
        "Blackened earth where farms once stood. Craters. A broken plow.\n"
        "Coalition artillery practiced here until Tolkeen's wards pushed\n"
        "the line back - for now.\n\n"
        "The approach road is north. Ruined suburbs lie east.");
    set_exits( ([
        "north" : "/domains/tolkeen/areas/tolkeen_approach",
        "east"  : "/domains/tolkeen/areas/tolkeen_ruined_suburb"
    ]) );
    set_listen("default", "Nothing alive. Wind only.");
    set_smell("default", "Burned soil.");
}

void reset() {
    ::reset();
    if(!present("cs siege scout", this_object()) && !random(2))
        clone_object("/domains/tolkeen/monsters/cs_siege_scout")->move(this_object());
}
