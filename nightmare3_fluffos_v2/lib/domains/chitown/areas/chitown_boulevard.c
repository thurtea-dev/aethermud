// /domains/chitown/areas/chitown_boulevard.c
// Main boulevard of Chi-Town. Moxim spawns here.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

#define MOXIM_PATH "/domains/Praxis/monsters/moxim"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 1);
    set_short("Chi-Town, Main Boulevard");
    set_long(
        "Clean ordered streets wider than most pre-Rifts highways. Coalition\n"
        "propaganda posters cover every surface: smiling human soldiers, the\n"
        "CS skull emblem, slogans about strength and purity. Patrol drones\n"
        "circle overhead in slow patterns. CS soldiers walk in pairs along\n"
        "the sidewalks, scanning faces.\n\n"
        "Despite the surveillance and propaganda, the boulevard hums with\n"
        "commerce and movement. Citizens in Coalition-approved clothing move\n"
        "quickly and purposefully. A covered shopping arcade opens northeast.");
    set_exits( ([
        "south" : "/domains/chitown/areas/chitown_gate",
        "north" : "/domains/chitown/areas/cs_command_plaza",
        "east" : "/domains/chitown/areas/cs_recruitment",
        "west" : "/domains/chitown/areas/chitown_clinic",
        "northeast" : "/domains/chitown/areas/cs_shopping_arcade"
    ]) );
    set_listen("default",
        "Coalition anthems from speakers, drone engines overhead, and the "
        "shuffling of citizens moving quickly.");
    set_smell("default",
        "Synthetic air freshener. Everything smells the same.");
    set_items( ([
        "posters" : "Coalition propaganda. Humans in armor. HUMANITY FIRST. WE ARE THE FUTURE.",
        "drones" : "Patrol drones. They record everything.",
        "soldiers" : "CS soldiers in full armor. They watch you.",
        "speakers" : "Mounted on every lamppost. Playing the Coalition anthem."
    ]) );
}

void reset() {
    object *obs;
    int count;
    int j;
    ::reset();
    if(!present("moxim", this_object()))
        clone_object(MOXIM_PATH)->move(this_object());
    /* Intermittent blue rift to the ocean off Splynn: one reset in
       four, collapsing on its own after 120 seconds. The blue tint
       is the only sign it is not ordinary rift work. */
    if(!present("blue rift", this_object()) && !random(4))
        clone_object("/std/blue_rift_portal")->move(this_object());
    if(!present("cs patrol officer", this_object()))
        clone_object("/domains/chitown/npcs/cs_patrol")->move(this_object());
    if(!present("coalition sergeant", this_object()))
        clone_object("/domains/chitown/npcs/cs_sergeant")->move(this_object());
    if(!present("cs grunt", this_object()))
        clone_object("/domains/chitown/npcs/cs_grunt")->move(this_object());
    if(!present("cs dog boy", this_object()))
        clone_object("/domains/chitown/npcs/cs_dog_boy")->move(this_object());
    obs   = all_inventory(this_object());
    count = 0;
    for(j = 0; j < sizeof(obs); j++) {
        if(living(obs[j]) &&
           strsrch((string)obs[j]->query_short(), "Chi-Town citizen") != -1)
            count++;
    }
    while(count < 2) {
        clone_object("/domains/chitown/npcs/cs_citizen")->move(this_object());
        count++;
    }
}
