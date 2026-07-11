/* /domains/Horton/areas/horton_bandit_camp.c
   Rough camp east of the scrub ridge. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Bandit Camp");
    set_long(
        "A ring of canvas and scrap metal around a cold firepit. Empty\n"
        "ration packs and spent shell casings litter the dirt. Someone\n"
        "watches the ridge from here when they are not hunting travelers.\n\n"
        "Scrub ridge is west. Cornfield ruins north.");
    set_exits( ([
        "west"  : "/domains/Horton/areas/horton_scrub_ridge",
        "north" : "/domains/Horton/areas/horton_cornfield_ruins"
    ]) );
    set_listen("default", "Canvas flapping. A radio hissing static.");
    set_smell("default", "Old smoke and unwashed gear.");
    set_items( ([
        "firepit" : "Cold ash. Used often.",
        "casings" : "Mixed calibers. Scavenged weapons.",
        "canvas"  : "Stained tents weighted with rocks."
    ]) );
}

void reset() {
    object *inv;
    int n, i;

    ::reset();
    inv = all_inventory(this_object());
    n = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(inv[i] && inv[i]->id("trail bandit"))
            n++;
    }
    if(n < 1)
        clone_object("/domains/Horton/monsters/trail_bandit")->move(this_object());
    if(n < 2 && random(2) == 0)
        clone_object("/domains/Horton/monsters/trail_bandit")->move(this_object());
}
