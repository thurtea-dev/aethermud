/* /domains/Praxis/areas/splynn/alvurron/alvurron_blood_pit.c
   Alvurron, the Blood Pit. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, the Blood Pit");
    set_long(
        "A fighting pit sunk into the stone, ringed by tiers of stone\n"
        "benches worn smooth by centuries of spectators. Old stains have\n"
        "soaked so deep into the sand that no amount of raking hides them.\n"
        "A chalkboard of odds hangs by the gate, written in three scripts.\n"
        "The pit champion trains here between bouts. The warrens are north.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/alvurron/alvurron_warrens"
    ]) );
    set_listen("default",
        "The slow, heavy rhythm of the champion working a stone pell.");
    set_smell("default", "Blood, sand, and chalk dust.");
    set_items( ([
        "pit" : "Ten meters across, three deep, floored with dark sand.",
        "benches" : "Stone tiers polished by generations of gargoyle haunches.",
        "chalkboard" : "Odds on the next three bouts. The champion is heavily
favored in all of them.",
        "sand" : "Raked, but the stains stay."
    ]) );
}

void reset() {
    ::reset();
    if(!present("pit champion", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_champion")->move(this_object());
}
