// /domains/tolkeen/areas/tolkeen_crypt_hall.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_property("dark_room", 1);
    set_short("Tolkeen, Crypt Hall");
    set_long(
        "A long hall of burial niches older than the Free City. Some niches\n"
        "glow with residual PPE. Footprints in the dust go both ways.\n\n"
        "The stair is up. A ward chamber opens east. Deeper crypt north.");
    set_exits( ([
        "up"    : "/domains/tolkeen/areas/tolkeen_crypt_entrance",
        "east"  : "/domains/tolkeen/areas/tolkeen_crypt_ward",
        "north" : "/domains/tolkeen/areas/tolkeen_crypt_depths"
    ]) );
    set_listen("default", "Whispers that stop when you listen hard.");
    set_smell("default", "Dust and old incense.");
}

void reset() {
    ::reset();
    if(!present("crypt shade", this_object()))
        clone_object("/domains/tolkeen/monsters/crypt_shade")->move(this_object());
}
