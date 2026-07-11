// /domains/chitown/areas/chitown_alley.c
// A dark alley off the main burb street.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("dark_room", 1);
    set_short("Dark Alley");
    set_long(
        "A narrow passage between crumbling walls. Broken glass underfoot.\n"
        "Laundry strung between buildings blocks what little light reaches\n"
        "this far from the street. The shadows are deep and still.\n\n"
        "A figure leans against the far wall, coat pulled close.");
    set_exits( ([
        "east" : "/domains/chitown/areas/chitown_burbs",
        "west" : "/domains/chitown/areas/chitown_burb_shanty"
    ]) );
    set_listen("default",
        "Dripping water. The muffled noise of the burb street east of here.");
    set_smell("default",
        "Mildew, old garbage, and something chemical.");
    set_items( ([
        "walls" : "Crumbling ferrocrete, stained and cracked.",
        "glass" : "Broken glass ground into the dirt floor.",
        "figure" : "A lean figure in a long coat. Watching you without making it obvious.",
        "coat" : "Long, heavy, concealing."
    ]) );
}

void reset() {
    ::reset();
    if(!present("contraband dealer", this_object()))
        clone_object("/domains/chitown/npcs/contraband_dealer")->move(this_object());
}
