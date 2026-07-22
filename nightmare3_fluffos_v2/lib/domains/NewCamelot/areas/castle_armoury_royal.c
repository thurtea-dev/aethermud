/* /domains/NewCamelot/areas/castle_armoury_royal.c
   Camelot Castle - Royal Armory. */

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Camelot Castle - Royal Armory");
    set_long(
        "The royal armory of Camelot Castle. Weapon racks fill the vaulted\n"
        "chamber: lances, swords and shields of the Order, each blessed\n"
        "and numbered. At the center, on a stand of black iron, rests the\n"
        "Order's dragonfire lance when it is not at war. The courtyard is\n"
        "east.");
    set_listen("default",
        "Your own footsteps, unnervingly loud.");
    set_smell("default",
        "Oil, steel, and a faint smell of sulfur near the lance stand.");
    set_items( ([
        "racks" : "Arms enough for a company of knights.",
        "stand" : "Black iron, worked with a coiled dragon."
    ]) );
    set_exits( ([
        "east" : "/domains/NewCamelot/areas/castle_courtyard"
    ]) );
}

void reset() {
    int taken;

    ::reset();
    taken = 0;
    catch(taken = (int)UNIQUE_ITEMS_D->query_taken("dragonfire_lance"));
    if(!present("quartermaster knight", this_object()))
        new("/domains/NewCamelot/npcs/quartermaster_knight")->move(this_object());
    if(!taken && !present("dragonfire lance", this_object()))
        new("/domains/NewCamelot/equipment/dragonfire_lance")->move(this_object());
}
