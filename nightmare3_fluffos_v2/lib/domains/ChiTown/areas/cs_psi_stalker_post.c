// /domains/ChiTown/areas/cs_psi_stalker_post.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Chi-Town, Psi-Stalker Post");
    set_long(
        "A sparse watch room. No decorations. A map of ley activity around\n"
        "Chi-Town is pinned to the wall with magnetic studs. The air feels\n"
        "thin, as if something is always listening.\n\n"
        "The ISS checkpoint is south.");
    set_exits( ([
        "south" : "/domains/ChiTown/areas/cs_iss_checkpoint"
    ]) );
    set_listen("default", "Almost nothing. Then a soft intake of breath.");
    set_smell("default", "Cold metal and old sweat.");
    set_items( ([
        "map" : "Ley lines and known nexus points marked in red grease pencil."
    ]) );
}

void reset() {
    ::reset();
    if(!present("cs psi-stalker", this_object()))
        clone_object("/domains/ChiTown/npcs/cs_psi_stalker")->move(this_object());
}
