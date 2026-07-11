// /domains/tolkeen/areas/tolkeen_crypt_entrance.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Crypt Stair");
    set_long(
        "A stone stair under Liberation Square. Ley light seeps through\n"
        "cracks. A sign in three languages: AUTHORIZED DIGGERS ONLY.\n"
        "Refugees ignore signs when the city needs PPE stones.\n\n"
        "Up returns to the square. The crypt hall continues down.");
    set_exits( ([
        "up"   : "/domains/tolkeen/areas/tolkeen_square",
        "down" : "/domains/tolkeen/areas/tolkeen_crypt_hall"
    ]) );
    set_listen("default", "Dripping water and a deep ward pulse.");
    set_smell("default", "Cold earth.");
}
