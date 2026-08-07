#include <std.h>
#include <rooms.h>

#define MON_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("outdoors", 1);
    set_property("wilderness", 1);
    set_short("Rift overgrown jungle");
    set_long(
        "Humidity rises in this tangled green belt. Rift-mutated vines choke\n"
        "the trees. The jungle grows thicker to the north and east, while\n"
        "broken roads offer easier travel south and west.");
    set_items( ([
        "jungle"     : "A rift-choked rain forest.",
        "vegetation" : "Plants you cannot identify from pre-Rifts botany texts.",
        "vines"      : "Thick vines pulsing with faint bioluminescence."
    ]) );
    set_exits( ([
        "northeast" : "/domains/Praxis/rain_forest",
        "southwest" : "/domains/Praxis/wild2"
    ]) );
}

void reset() {
    int i;

    ::reset();
    if(!present("jungle stalker", this_object())) {
        i = 1 + random(2);
        while(i-- > 0)
            clone_object(MON_PATH + "jungle_stalker.c")->move(this_object());
    }
}
