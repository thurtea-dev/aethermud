// /domains/tolkeen/areas/tolkeen_library.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Tolkeen, War Library");
    set_long(
        "Shelves of salvaged books and data crystals. A section labeled\n"
        "COALITION TACTICS is heavily used. Quiet is enforced by a ward that\n"
        "stings loud talkers.\n\n"
        "The mage quarter is south.");
    set_exits( ([
        "south" : "/domains/tolkeen/areas/tolkeen_mage_quarter"
    ]) );
    set_listen("default", "Pages turning. Soft ward static.");
    set_smell("default", "Old paper and dust.");
    set_items( ([
        "shelves" : "War, ley theory, and banned Coalition manuals.",
        "crystals" : "Data crystals in padded racks."
    ]) );
}

void reset() {
    ::reset();
    if(!present("tolkeen mage", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_mage")->move(this_object());
}
