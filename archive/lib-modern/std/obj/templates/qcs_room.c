/* QCS room template - placeholders replaced at creation time. */
#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("QCS_SHORT");
    set_long("QCS_LONG");
    set_exits( ([]) );
}
