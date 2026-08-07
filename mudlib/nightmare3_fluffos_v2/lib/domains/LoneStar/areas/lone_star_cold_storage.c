/* /domains/LoneStar/areas/lone_star_cold_storage.c
   Lone Star failed-specimen cold storage, west of the incubation gallery.
   Where lines that did not meet spec are held pending disposal.
   Connects east to incubation. */

#define MON_PATH "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Cold Storage");
    set_long(
        "Past the insulated door the temperature drops hard. Specimen racks\n"
        "fill the room, three tiers deep, each cradle holding a sealed sleeve\n"
        "tagged with a line code and a single stamped word: SURPLUS.\n\n"
        "These are the branches that did not meet spec. Coalition procedure\n"
        "holds them here pending disposal, and Coalition procedure assumes\n"
        "the cold keeps them held.\n\n"
        "Two cradles on the bottom tier are empty. Their sleeves are on the\n"
        "floor, opened from the inside.\n\n"
        "The incubation gallery is back east.");
    set_exits( ([
        "east" : "/domains/LoneStar/areas/lone_star_incubation"
    ]) );
    set_listen("default",
        "The compressor cycling, and between cycles a silence heavy enough\n"
        "that your own breathing sounds wrong.");
    set_smell("default",
        "Freezer burn, sealed plastic, and something sweet underneath that\n"
        "the cold has not quite stopped.");
    set_items( ([
        "racks"    : "Three tiers of specimen cradles running the length of\n"
                     "the room.",
        "sleeves"  : "Sealed specimen sleeves, each tagged with a line code\n"
                     "and stamped SURPLUS.",
        "cradles"  : "Molded holders, one per sleeve. Two on the bottom\n"
                     "tier are empty.",
        "floor"    : "Two opened sleeves lie on the floor below the empty\n"
                     "cradles. The seals are torn outward.",
        "tags"     : "Line codes, dates, and the same stamped word on every\n"
                     "one of them.",
        "door"     : "A heavy insulated door back to the gallery. It closes\n"
                     "on its own weight."
    ]) );
}

void reset() {
    ::reset();
    if(!present("failed specimen", this_object()))
        clone_object(MON_PATH + "ls_failed_specimen.c")->move(this_object());
}
