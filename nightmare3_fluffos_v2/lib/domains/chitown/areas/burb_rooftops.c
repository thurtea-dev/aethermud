// /domains/chitown/areas/burb_rooftops.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Burb Rooftops");
    set_long(
        "Flat roofs and makeshift bridges of plank and cable stretch between\n"
        "tenement tops. From here the Chi-Town wall fills the northern sky,\n"
        "turrets like black teeth. A careful climber can see patrol routes.\n\n"
        "A stair leads down to the tenements. West, a low wall-walk follows\n"
        "the shadow of the city wall.");
    set_exits( ([
        "down" : "/domains/chitown/areas/burb_tenements",
        "west" : "/domains/chitown/areas/burb_wall_shadow"
    ]) );
    set_listen("default",
        "Wind across sheet metal, a distant hovertrain horn.");
    set_smell("default",
        "Tar, dust, and kitchen smoke rising from below.");
    set_items( ([
        "wall" : "The Coalition wall. Closer from up here, and no kinder.",
        "turrets" : "They track movement. Best not to wave.",
        "bridges" : "Planks and cable. Trust them at your own risk."
    ]) );
}
