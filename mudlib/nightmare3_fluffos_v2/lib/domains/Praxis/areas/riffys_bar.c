// /d/Praxis/areas/riffys_bar.c
// Redirect stub: Rocky's Bar is now at rockys_bar.c
// This file exists only to prevent broken link errors.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Rocky's Bar");
    set_long("You are in Rocky's Bar. Type: go east");
    set_exits( (["east" : "/domains/Praxis/areas/monument_square"]) );
}
