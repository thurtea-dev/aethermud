/* /domains/LoneStar/areas/lone_star_canteen.c
   Lone Star support town canteen, off Support Row.
   Off-shift drinking hole for complex staff; safe room, talk and trade.
   Connects west to Support Row. */

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Lone Star: The Dry Kennel");
    set_long(
        "A low room with a poured concrete floor, a counter salvaged from a\n"
        "pre-Rifts diner, and eight tables that have all been repaired at\n"
        "least once. A hand-lettered board over the counter names the place\n"
        "The Dry Kennel. Complex staff drink here off shift because it is\n"
        "the only thing within walking distance of the gate.\n\n"
        "Shoulder patches are pinned in rows along the back wall, one for\n"
        "every crew that has rotated through the complex. Some of the older\n"
        "ones are for units that no longer exist.\n\n"
        "Support Row is back west.");
    set_exits( ([
        "west" : "/domains/LoneStar/areas/lone_star_support_row"
    ]) );
    set_listen("default",
        "Low talk that drops a level whenever the door opens, and a fan\n"
        "turning somewhere behind the counter.");
    set_smell("default",
        "Frying oil, spilled beer soaked into concrete, and cigarette smoke\n"
        "that has been in the walls for years.");
    set_items( ([
        "counter" : "A pre-Rifts diner counter, salvaged and rebolted. The\n"
                    "chrome edge is worn through to the base metal.",
        "board"   : "Hand-lettered: THE DRY KENNEL. Someone with a steady\n"
                    "hand and no training made it.",
        "patches" : "Shoulder patches pinned in rows along the back wall,\n"
                    "one per crew rotated through the complex.",
        "tables"  : "Eight tables, each repaired at least once, none of them\n"
                    "matching.",
        "wall"    : "The patch wall. The older rows are for units that were\n"
                    "not rotated out so much as used up.",
        "floor"   : "Poured concrete, hosed rather than mopped."
    ]) );
}

void reset() {
    ::reset();
    if(!present("canteen keeper", this_object()))
        clone_object(NPC_PATH + "ls_canteen_keeper.c")->move(this_object());
}
