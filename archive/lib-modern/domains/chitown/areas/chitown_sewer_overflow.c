// /domains/chitown/areas/chitown_sewer_overflow.c
// Old overflow chamber at the end of the Chi-Town sewer run.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_short("Chi-Town Sewer Overflow Chamber");
    set_long(
        "A wider chamber where the sewer run widens into a shallow pool.\n"
        "Broken ferrocrete slabs form a crude walkway above the water. A rusted\n"
        "maintenance hatch is welded shut in the ceiling. Coalition patrols\n"
        "rarely come this far beneath the burbs.\n\n"
        "A side passage leads east into a wider cavern.\n"
        "The main tunnel is west.");
    set_exits( ([
        "west" : "/domains/chitown/areas/chitown_sewer_run",
        "east" : "/domains/chitown/areas/chitown_hydra_lair"
    ]) );
    set_listen("default",
        "Echoing drips. A distant rumble from machinery above the city.");
    set_smell("default",
        "Rot, rust, and the sharp tang of old chemical waste.");
    set_items( ([
        "pool" : "Shallow black water. Something glints beneath the surface.",
        "hatch" : "A welded maintenance hatch. No way up from here.",
        "slabs" : "Broken walkway slabs, slippery with algae."
    ]) );
}

int do_search(string str) {
    object crate;

    if(!str || strsrch(lower_case(str), "pool") == -1)
        return 0;
    if((int)query_property("pool_searched")) {
        write("You already searched the pool.\n");
        return 1;
    }
    set_property("pool_searched", 1);
    write("You wade into the pool and pull up a waterlogged supply crate.\n");
    crate = clone_object("/domains/Praxis/equipment/supply_crate");
    if(crate) crate->move(this_object());
    return 1;
}

void init() {
    ::init();
    add_action("do_search", "search");
}
