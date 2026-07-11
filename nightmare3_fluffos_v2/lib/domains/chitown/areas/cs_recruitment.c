// /domains/chitown/areas/cs_recruitment.c
// CS Recruitment Center.

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/chitown/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no steal", 1);
    set_short("Chi-Town, CS Recruitment Center");
    set_long(
        "Gleaming white walls and bright lighting. Recruitment holos play on\n"
        "every surface: soldiers in full Dead Boy armor, laser rifles raised.\n"
        "A CS skull emblem on polished steel dominates the far wall.\n\n"
        "A CS Recruiter sits at a desk in the center, reviewing personnel\n"
        "files with crisp efficiency. The door is watched by automated\n"
        "scanning equipment. The barracks lie south.");
    set_exits( ([
        "west"  : "/domains/chitown/areas/chitown_boulevard",
        "south" : "/domains/chitown/areas/cs_barracks"
    ]) );
    set_listen("default",
        "Recruitment propaganda playing in a loop. The recruiter typing.");
    set_smell("default",
        "Clean. Antiseptic. Institutional.");
    set_items( ([
        "holos" : "Coalition recruitment footage. ACTION. POWER. SECURITY.",
        "emblem" : "The CS skull on polished steel. Imposing.",
        "desk" : "The recruiter's desk. Personnel files and a palm-print scanner.",
        "scanner" : "Biometric entry scanner. It already knows who you are."
    ]) );
}

void reset() {
    ::reset();
    if(!present("cs recruiter", this_object()))
        clone_object(NPC_PATH+"cs_recruiter")->move(this_object());
}
