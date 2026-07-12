// /d/Praxis/areas/splynn/preserve_thornbrake.c
// Splynn Preserves hunting loop, R2: dense thornbrush ambush country.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Thorn Brake");
    set_long(
        "The trail chokes down into a brake of black thornbrush, each spine\n"
        "as long as a finger and hooked at the tip. The growth is dense\n"
        "enough to snag armor and slow a running body, which is precisely\n"
        "why the hunters cultivated it here. Nothing crosses this ground at\n"
        "speed.\n\n"
        "Narrow gaps wind through the thorns, and every gap is a place where\n"
        "something could be waiting. The brake opens east back toward the\n"
        "trail and drops away south into lower, wetter ground.");
    set_exits( ([
        "east"  : "/domains/Praxis/areas/splynn/preserve_trail",
        "south" : "/domains/Praxis/areas/splynn/preserve_waterhole"
    ]) );
    set_listen("default",
        "The dry rattle of thorns shifting, though there is no wind, and a\n"
        "chittering that stops the moment you try to place it.");
    set_smell("default",
        "Sap that stings the back of the throat, and under it a sweetish\n"
        "rot where something died tangled in the brush.");
    set_items( ([
        "thorns" : "Black spines the length of a finger, hooked so they hold\n"
                   "what they catch. Cloth and softer armor tear on them.",
        "gaps"   : "Winding paths through the thornbrush, just wide enough for\n"
                   "one body. Ideal ground for an ambush.",
        "brush"  : "A wall of hooked thornbrush, deliberately dense. Someone\n"
                   "wanted this to be slow, killing ground."
    ]) );
}

void reset() {
    ::reset();
    if(random(10) < 6 && !present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
    if(random(10) < 4 && !present("xiticix warrior", this_object()))
        clone_object(MONSTER_PATH+"xiticix_warrior.c")->move(this_object());
}
