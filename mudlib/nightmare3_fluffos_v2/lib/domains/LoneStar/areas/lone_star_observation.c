/* /domains/LoneStar/areas/lone_star_observation.c
   Lone Star observation booth above the incubation gallery.
   Control room for the vat floor; connects the gene vault to the vats.
   Connects down to incubation, south to the gene vault. */

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Lone Star: Observation Booth");
    set_long(
        "A glassed-in booth cantilevered over the vat floor, close enough to\n"
        "the ceiling that the hanging cable bundles pass at eye level. A\n"
        "curved console runs the width of the window: tank vitals, feed rates,\n"
        "and a bank of small screens cycling through the rows below.\n\n"
        "One screen is fixed rather than cycling. It shows a single tank, and\n"
        "the readout beside it has been flagged in red and left flagged.\n\n"
        "A steel stair goes down to the gallery floor. A short passage south\n"
        "leads to the gene vault.");
    set_exits( ([
        "down"  : "/domains/LoneStar/areas/lone_star_incubation",
        "south" : "/domains/LoneStar/areas/lone_star_gene_vault"
    ]) );
    set_listen("default",
        "The booth is sealed, so the pumps below come through as a low\n"
        "pressure in the floor rather than a sound. The consoles hum.");
    set_smell("default",
        "Filtered air, warm electronics, and cold coffee left on the\n"
        "console.");
    set_items( ([
        "console" : "A curved control console spanning the window. Tank\n"
                    "vitals, feed rates, and override switches under covers.",
        "window"  : "Angled glass looking straight down the vat rows. From\n"
                    "up here the gallery reads as inventory.",
        "screens" : "A bank of monitors cycling the tank rows below. All\n"
                    "but one are cycling.",
        "readout" : "Flagged red and left that way. Whatever is in that\n"
                    "tank stopped matching its projection some time ago.",
        "coffee"  : "A cold cup on the console edge, ring-stained into the\n"
                    "surface from long use.",
        "screen"  : "The fixed screen. One tank, held in view, not cycling\n"
                    "with the rest."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Coalition scientist", this_object()))
        clone_object(NPC_PATH + "coalition_scientist.c")->move(this_object());
}
