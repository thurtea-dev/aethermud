// /domains/chitown/areas/cs_outfitter_shop.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Chi-Town, Outfitter");
    set_long(
        "Racks of gray and black civilian clothing. CS seals on every tag.\n"
        "A mirror bolted to the wall. Nothing flashy. Nothing unapproved.\n\n"
        "The ration depot is south.");
    set_exits( ([
        "south" : "/domains/chitown/areas/cs_ration_depot"
    ]) );
    set_listen("default", "Fabric rustle and a quiet radio.");
    set_smell("default", "New cloth and starch.");
}

void reset() {
    ::reset();
    if(!present("cs outfitter", this_object()))
        clone_object("/domains/chitown/npcs/cs_outfitter")->move(this_object());
}
