/* /domains/Lazlo/areas/lazlo_shoreline.c
   The narrow stone path along the Lake of the Gifted, east of the city gate.
   City-edge feel. Feral dogs work this strip for scraps from the gate traffic.
   Entry-level combat pocket for levels 2-4. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Lazlo, Lake Shore Path");
    set_long(
        "A narrow stone path worn smooth by decades of foot traffic, running\n"
        "along the eastern face of Lazlo's outer wall. To the west the wall\n"
        "rises solid and close. To the east the Lake of the Gifted stretches\n"
        "out, its surface reflecting gray sky, the water clear enough to\n"
        "see the ley line threads running along the bottom.\n\n"
        "The ground between the path and the waterline is littered with\n"
        "refuse from the gate market: scraps, broken crates, discarded\n"
        "salvage. Something rustles in the debris.");
    set_exits( ([
        "west" : "/domains/Lazlo/areas/lazlo_gate"
    ]) );
    set_listen("default",
        "Water lapping against stone, the creak of old wood in the debris pile, "
        "and an occasional low growl.");
    set_smell("default",
        "Lake water, damp stone, and the sour smell of market refuse.");
    set_items( ([
        "wall"    : "Lazlo's outer wall, solid fitted stone. Gate arch visible to the west.",
        "lake"    : "The Lake of the Gifted. Clear water with ley line threads visible in the depths.",
        "path"    : "Worn smooth by foot traffic. Narrow but passable.",
        "debris"  : "Broken crates, discarded salvage, and refuse from the market. "
                    "Something is living in there.",
        "refuse"  : "Market scraps. Whoever cleans the gate plaza does not clean this far.",
        "water"   : "Clear, cold lake water. The ley line threads visible beneath the surface."
    ]) );
}

void reset() {
    object *obs;
    int count;
    int j;
    ::reset();
    obs   = all_inventory(this_object());
    count = 0;
    for(j = 0; j < sizeof(obs); j++) {
        if(living(obs[j]) &&
           strsrch((string)obs[j]->query_short(), "feral dog") != -1)
            count++;
    }
    while(count < 2) {
        clone_object("/domains/Lazlo/monsters/lazlo_feral_dog")->move(this_object());
        count++;
    }
}
