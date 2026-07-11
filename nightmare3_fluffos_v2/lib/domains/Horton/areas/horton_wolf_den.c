/* /domains/Horton/areas/horton_wolf_den.c
   Wolf den north of deep forest / ancient grove. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("wilderness", 1);
    set_short("Wolf Den");
    set_long(
        "A cave mouth under a root wall. Bones and fur litter the floor.\n"
        "The smell is thick. This is not a place to linger unless you came\n"
        "to finish a fight.\n\n"
        "Ancient grove south. Deep forest southwest.");
    set_exits( ([
        "south"     : "/domains/Horton/areas/horton_ancient_grove",
        "southwest" : "/domains/Horton/areas/horton_forest_deep"
    ]) );
    set_listen("default", "Breathing that is not yours.");
    set_smell("default", "Wet fur and old kills.");
    set_items( ([
        "bones" : "Deer and something larger.",
        "fur"   : "Tufts caught on roots.",
        "cave"  : "Shallow. Pack space, not a deep system."
    ]) );
}

void reset() {
    object *inv;
    int n, i;

    ::reset();
    inv = all_inventory(this_object());
    n = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(inv[i] && inv[i]->id("plains wolf"))
            n++;
    }
    if(n < 2)
        clone_object("/domains/Praxis/monsters/plains_wolf")->move(this_object());
    if(n < 3)
        clone_object("/domains/Praxis/monsters/plains_wolf")->move(this_object());
}
