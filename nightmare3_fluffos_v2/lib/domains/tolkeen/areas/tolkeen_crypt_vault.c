// /domains/tolkeen/areas/tolkeen_crypt_vault.c

#include <std.h>
#include <rooms.h>

#define EQ_PATH "/domains/tolkeen/equipment/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Crypt Vault");
    set_long(
        "A small vault. Empty weapon racks. A stone pedestal where a charm\n"
        "sometimes rests - left as payment to the dead, or forgotten by a\n"
        "digger who ran.\n\n"
        "The crypt depths are south.");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_crypt_depths"
    ]) );
    set_listen("default", "Silence pressing on the ears.");
    set_smell("default", "Cold air.");
}

void reset() {
    ::reset();
    if(!present("ward charm", this_object()))
        clone_object(EQ_PATH + "ward_charm.c")->move(this_object());
}
