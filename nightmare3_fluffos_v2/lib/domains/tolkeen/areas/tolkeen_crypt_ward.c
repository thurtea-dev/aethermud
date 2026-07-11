// /domains/tolkeen/areas/tolkeen_crypt_ward.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_short("Tolkeen, Ward Chamber");
    set_long(
        "A circular chamber. Runes on the floor feed the city walls above.\n"
        "Techno-wizard conduits snake into the stone. Damaging this place\n"
        "would drop Tolkeen's shields - which is why it is guarded.\n\n"
        "The crypt hall is west.");
    set_exits( ([
        "west" : "/domains/tolkeen/areas/tolkeen_crypt_hall"
    ]) );
    set_listen("default", "A steady magical heartbeat.");
    set_smell("default", "Hot copper and ozone.");
    set_items( ([
        "runes" : "They brighten when Coalition guns fire above.",
        "conduits" : "TW cabling fused into bedrock."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ward golem", this_object()))
        clone_object("/domains/tolkeen/monsters/tolkeen_ward_golem")->move(this_object());
}
