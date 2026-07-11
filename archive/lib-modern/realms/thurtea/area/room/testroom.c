/* /realms/thurtea/area/room/testroom.c */
/* QCS room by thurtea */

/* QCS room template - placeholders replaced at creation time. */
#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("A new area");
    set_long("A bare room waiting for description.");
    add_exit("south", "/domains/Praxis/rifts_welcome");
}
