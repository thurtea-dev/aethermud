/* /domains/NewCamelot/areas/blacksmith_forge.c
   Blacksmith's Forge. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Blacksmith's Forge");
    set_long(
        "The forge of New Camelot, hot as a dragon's gullet. An anvil the\n"
        "size of a table sits at the center, and the walls are hung with\n"
        "blades in every stage of finish: medieval patterns edged in modern\n"
        "mega-damage alloys. Market Street is south.");
    set_listen("default",
        "The ring of hammer on steel and the roar of the forge fire.");
    set_smell("default",
        "Coal, quenching oil, and scorched leather.");
    set_items( ([
        "anvil" : "Scarred by decades of work. Salvaged from a pre-Rifts naval yard.",
        "blades" : "Longswords, falchions and lance heads, forged in the old shapes from new alloys.",
        "bellows" : "Motorized, but the smith pumps them by hand anyway. Tradition, she says."
    ]) );
    set_exits( ([
        "south" : "/domains/NewCamelot/areas/market_street1"
    ]) );
}

void reset() {
    ::reset();
    if(!present("blacksmith", this_object()))
        new("/domains/NewCamelot/npcs/camelot_blacksmith")->move(this_object());
}
