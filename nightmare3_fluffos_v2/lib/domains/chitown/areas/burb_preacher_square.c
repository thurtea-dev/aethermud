// /domains/chitown/areas/burb_preacher_square.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Preacher's Square");
    set_long(
        "A cleared patch of packed dirt where three lanes meet. Someone\n"
        "dragged a crate into the center as a makeshift pulpit. Burbies\n"
        "pass through without lingering unless the shouting is good.\n\n"
        "The soup kitchen is west. South leads to a community water pump.\n"
        "Northeast, a Dog Boy checkpoint watches the approach to the wall.");
    set_exits( ([
        "west"      : "/domains/chitown/areas/burb_soup_kitchen",
        "south"     : "/domains/chitown/areas/burb_water_pump",
        "northeast" : "/domains/chitown/areas/burb_dogboy_checkpoint"
    ]) );
    set_listen("default",
        "A raised voice cutting through street noise, then the murmur of the crowd.");
    set_smell("default",
        "Dust, smoke from nearby barrel fires, and cheap incense.");
    set_items( ([
        "crate" : "A shipping crate stamped with a faded Coalition inventory code.",
        "lanes" : "Three muddy paths converging here from the denser burbs."
    ]) );
}

void reset() {
    ::reset();
    if(!present("street preacher", this_object()))
        clone_object("/domains/chitown/npcs/burb_preacher")->move(this_object());
}
