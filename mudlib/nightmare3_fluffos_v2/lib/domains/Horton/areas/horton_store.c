/* /domains/Horton/areas/horton_store.c
   Horton General Store -- survival supplies, no questions asked. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Horton General Store");
    set_long(
        "Stocked with survival basics. Shelves of canned goods, pre-Rifts\n"
        "emergency rations, coils of rope, basic tools and first aid supplies.\n"
        "Everything is labeled and priced. The proprietor trades in everything\n"
        "and asks about nothing. A hand radio charges behind the counter.\n\n"
        "Type 'list' to see available goods. Type 'buy <item>' to purchase.");
    set_exits( ([
        "east" : "/domains/Horton/areas/horton_main_street"
    ]) );
    set_listen("default",
        "The creak of shelves. Quiet. The proprietor does not make small talk.");
    set_smell("default",
        "Preservatives, canvas, and machine oil.");
    set_items( ([
        "shelves"   : "Well-stocked. Organized. Someone keeps this place running.",
        "goods"     : "Survival basics. Type list to see what is available.",
        "counter"   : "A wooden counter with a dented cash register.",
        "radio"     : "A personal radio on a charging stand. For sale.",
        "proprietor": "A compact, quiet person who has heard everything twice."
    ]) );
}

void reset() {
    object npc;
    if(!present("shopkeeper", this_object())) {
        npc = clone_object("/domains/Horton/npcs/horton_shopkeeper");
        npc->move(this_object());
    }
    ::reset();
}
