// /domains/tolkeen/areas/tolkeen_ruined_suburb.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Ruined Suburb");
    set_long(
        "Collapsed houses and a street cracked by ley backlash. Graffiti\n"
        "curses the Coalition in three languages. Good cover for scouts -\n"
        "on both sides.\n\n"
        "Scorched fields are west. A trench line continues east.");
    set_exits( ([
        "west" : "/domains/tolkeen/areas/tolkeen_scorched_field",
        "east" : "/domains/tolkeen/areas/tolkeen_cs_trench"
    ]) );
    set_listen("default", "Settling rubble. A distant radio hiss.");
    set_smell("default", "Mold and ash.");
}

void reset() {
    ::reset();
    if(!present("cs siege scout", this_object()))
        clone_object("/domains/tolkeen/monsters/cs_siege_scout")->move(this_object());
}
