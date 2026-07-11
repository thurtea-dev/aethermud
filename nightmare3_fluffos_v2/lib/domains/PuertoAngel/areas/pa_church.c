/* /domains/PuertoAngel/areas/pa_church.c
   Abandoned church at the north edge of the residential district.
   The vampires avoid it. Why is unclear -- they do not discuss it.
   Connects south to the residential district.
   Silver cross item spawns here: useful against vampires. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

#define CROSS_PATH "/domains/PuertoAngel/equipment/silver_cross"

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_property("rest_allowed", 1);
    set_short("Abandoned Church");
    set_long(
        "A whitewashed colonial church at the north edge of town. Old.\n"
        "Pre-Rifts, probably pre-electric. The interior is dim, lit by\n"
        "sunlight through cracked high windows and a handful of candles\n"
        "someone has left burning at the altar.\n\n"
        "It is the only building in Puerto Angel the vampires do not enter.\n"
        "Whether this is custom, discomfort, or something more, no one who\n"
        "lives here explains. The pews are dusty but intact. The altar cloth\n"
        "is old silk, faded white. A silver cross rests on the altar.\n\n"
        "The residential district is south.");
    set_exits( ([
        "south" : "/domains/PuertoAngel/areas/pa_residential"
    ]) );
    set_listen("default",
        "Wind through the cracked windows. Candles hissing. Your own breathing.\n"
        "Outside: absolute silence. This building sits in a pocket of quiet\n"
        "that feels different from the silence everywhere else in town.");
    set_smell("default",
        "Candle wax, old wood, and something clean. The smell stops at the door.");
    set_items( ([
        "pews"    : "Old wood, hand-carved. Dusty but intact. Someone kept them repaired "
                    "for years after no one sat in them.",
        "altar"   : "Stone base, silk cloth. A silver cross rests on it. "
                    "The vampires do not come near this room.",
        "candles" : "A cluster of candles at the altar, recently lit. "
                    "Someone visits this church. You do not know who.",
        "windows" : "High, cracked, with old colored glass in fragments. "
                    "What remains still catches the light.",
        "cross"   : "A silver cross on the altar. Heavy, old. Useful in this town.",
        "cloth"   : "Old silk altar cloth, faded to off-white. "
                    "The embroidery is still visible: a sunburst, not a cross."
    ]) );
}

void reset() {
    ::reset();
    if(!present("silver cross", this_object()))
        catch(new(CROSS_PATH)->move(this_object()));
}
