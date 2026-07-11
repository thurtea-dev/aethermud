// /domains/chitown/areas/chitown_sewer_entrance.c
// Sewer entrance beneath the Chi-Town burbs.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Chi-Town Burbs, Sewer Entrance");
    set_long(
        "A rusted grate opens onto a narrow landing at the top of a slick\n"
        "concrete stair. Cold air rises from the darkness below, carrying the\n"
        "smell of stagnant water and old waste. Iron rungs lead down into a\n"
        "narrow service tunnel beneath the burbs.\n\n"
        "The burbs are north, up the stair through the grate. The tunnel\n"
        "continues down.");
    set_exits( ([
        "north" : "/domains/chitown/areas/chitown_burbs",
        "down"  : "/domains/chitown/areas/chitown_sewer_run"
    ]) );
    set_listen("default",
        "Water dripping somewhere in the dark. A distant rumble you cannot place.");
    set_smell("default",
        "Stagnant water, mildew, and something rotting far below.");
    set_items( ([
        "grate" : "A rusted sewer grate above the stair. Daylight filters through.",
        "stair" : "Slick concrete steps leading down into darkness.",
        "darkness" : "Black below the landing. No safe path is marked.",
        "rungs" : "Iron rungs on the wall. They vanish into the dark."
    ]) );
}
