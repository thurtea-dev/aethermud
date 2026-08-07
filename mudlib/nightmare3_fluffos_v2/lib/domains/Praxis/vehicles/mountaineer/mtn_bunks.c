/* /d/Praxis/vehicles/mountaineer/mtn_bunks.c
   NG-V7 Mountaineer ATV: bunk room. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("vehicle_interior", 1);
    set_short("NG-V7 Mountaineer: Bunk Room");
    set_long(
        "A cramped sleeping compartment in the Mountaineer's tail. Four\n"
        "bunks fold out from the walls in two stacked pairs, each with a\n"
        "cargo net to keep the sleeper in place on rough ground. A narrow\n"
        "aisle between them ends at a chemical toilet behind an accordion\n"
        "door.\n\n"
        "The main cabin is forward (north).");
    set_exits( ([
        "north" : "/domains/Praxis/vehicles/mountaineer/mtn_cabin"
    ]) );
    set_listen("default",
        "Creaking bunk frames and the muffled sound of the street outside.");
    set_smell("default",
        "Old blankets and disinfectant.");
    set_items( ([
        "bunks" : "Four fold-out bunks with cargo nets. The bedding is worn\n"
                  "but clean.",
        "nets" : "Cargo netting to strap a sleeper down when the ATV is\n"
                 "climbing a forty-degree grade.",
        "toilet" : "A chemical toilet behind an accordion door. Best left\n"
                   "unexamined.",
        "door" : "A plastic accordion door, half off its track."
    ]) );
}

void reset() {
    ::reset();
}
