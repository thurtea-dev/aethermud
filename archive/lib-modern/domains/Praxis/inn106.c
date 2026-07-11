// /d/Praxis/inn106.c
// Survivors' Hostel, Manager's Office. Sal's workspace and personal quarters.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_property("rest_allowed", 1);
    set_short("Survivors' Hostel, Manager's Office");
    set_long(
        "A small upstairs office doubling as Sal's personal quarters. A\n"
        "steel desk dominates the room, covered in ledgers and supply\n"
        "manifests in Sal's cramped handwriting. A cot in the corner has\n"
        "a military sleeping bag on it, neatly rolled.\n\n"
        "Weapons are racked on one wall. Maps cover another. A radio\n"
        "transceiver on the desk crackles with distant traffic. Through\n"
        "a narrow window, Monument Square is visible below.");
    set_exits( ([ "down" : "/domains/Praxis/inn102" ]) );
    set_listen("default",
        "The crackle of the radio transceiver and the noise of Monument Square below.");
    set_smell("default",
        "Old paper, gun oil, and the faint smell of whoever last used the cot.");
    set_items( ([
        "desk"     : "Heavy steel, dented and scratched. Covered in ledgers and supply lists.",
        "ledgers"  : "Sal's records. Supply inventory, credit tallies, incident log. "
                     "Going back years.",
        "cot"      : "A military folding cot with a sleeping bag rolled tight. "
                     "This is where Sal sleeps when she sleeps.",
        "weapons"  : "A short-barreled shotgun and a holstered pistol, both on rack mounts. "
                     "Within easy reach of the desk.",
        "maps"     : "Regional maps covering one full wall. Routes marked, dangers noted, "
                     "supply caches circled in red.",
        "radio"    : "A pre-Rifts transceiver modified for post-Rifts frequencies. "
                     "It crackles with traffic from somewhere north of here.",
        "window"   : "Narrow. Monument Square is visible below, full of the usual traffic.",
        "bag"      : "Military sleeping bag, rolled tight. Efficient."
    ]) );
}

void reset() { ::reset(); }
