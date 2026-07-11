// /domains/chitown/areas/cs_iss_checkpoint.c
// Inner ISS checkpoint (memories: fortified city, D-bee danger).

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Chi-Town, ISS Checkpoint");
    set_long(
        "An Internal Security Service desk blocks the corridor. Retinal\n"
        "scanner. Weapon detector. A sign: ALL PERSONNEL SUBJECT TO SEARCH.\n"
        "Beyond the desk, restricted posts continue deeper into the city.\n\n"
        "The command plaza is southwest. Psi-Stalker post is north.\n"
        "SAMAS hangar is east.");
    set_exits( ([
        "southwest" : "/domains/chitown/areas/cs_command_plaza",
        "north"     : "/domains/chitown/areas/cs_psi_stalker_post",
        "east"      : "/domains/chitown/areas/cs_samas_hangar"
    ]) );
    set_listen("default", "Scanner tones and clipped ISS radio traffic.");
    set_smell("default", "Ozone from the detector arch.");
    set_items( ([
        "sign" : "ALL PERSONNEL SUBJECT TO SEARCH. No exceptions.",
        "scanner" : "A retinal scanner mounted at eye level."
    ]) );
}

void reset() {
    ::reset();
    if(!present("iss clerk", this_object()))
        clone_object("/domains/chitown/npcs/cs_iss_clerk")->move(this_object());
    if(!present("cs grunt", this_object()))
        clone_object("/domains/chitown/npcs/cs_grunt")->move(this_object());
}
