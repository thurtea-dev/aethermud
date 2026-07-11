// /domains/tolkeen/areas/tolkeen_crypt_depths.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_property("dark_room", 1);
    set_short("Tolkeen, Crypt Depths");
    set_long(
        "The hall narrows. Bones in niches here are older, unmarked. A cold\n"
        "draft pulls north toward a sealed vault door left ajar.\n\n"
        "The crypt hall is south. The vault is north.");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_crypt_hall",
        "north" : "/domains/tolkeen/areas/tolkeen_crypt_vault"
    ]) );
    set_listen("default", "Your pulse. Something else's.");
    set_smell("default", "Wet stone and grave dirt.");
}

void reset() {
    ::reset();
    if(!present("crypt shade", this_object()))
        clone_object("/domains/tolkeen/monsters/crypt_shade")->move(this_object());
}
