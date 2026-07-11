/* /domains/LoneStar/areas/lone_star_supply.c
   Lone Star complex supply depot.
   Requisitions and field gear for authorized personnel.
   Connects north to the complex plaza. */

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Lone Star: Supply Depot");
    set_long(
        "The supply depot is a warehouse-scaled room organized with Coalition\n"
        "precision. Shelves reach the ceiling, labeled with requisition codes.\n"
        "The front counter has two terminals and a supply officer who handles\n"
        "requests with the efficiency expected inside this complex.\n\n"
        "Standard Coalition field gear is available for purchase: e-clips,\n"
        "rations, medical supplies, and basic field tools. Heavy weapons\n"
        "require specialist authorization that is not easily obtained.\n\n"
        "The complex plaza is north.");
    set_exits( ([
        "north" : "/domains/LoneStar/areas/lone_star_plaza"
    ]) );
    set_listen("default",
        "The hum of climate-controlled storage. A forklift somewhere in the back.\n"
        "The beep of a terminal completing a transaction.");
    set_smell("default",
        "Sealed plastic packaging, Coalition-issue soap, and the faint chemical\n"
        "smell of field rations in bulk.");
    set_items( ([
        "shelves"  : "Floor-to-ceiling shelving with requisition codes on every row. "
                     "Everything has a place and everything is in it.",
        "counter"  : "Two service terminals and a supply officer behind a reinforced counter. "
                     "Purchases are logged automatically.",
        "rations"  : "Coalition field rations in sealed packs. "
                     "Balanced, calorie-dense, and tasting of absolutely nothing.",
        "gear"     : "Standard field gear: e-clips, med kits, tools. "
                     "Nothing exotic. Coalition standard only.",
        "forklift" : "An automated pallet mover visible through a gap in the shelving. "
                     "It has never made an error."
    ]) );
}

void reset() {
    ::reset();
    if(!present("supply officer", this_object()))
        clone_object(NPC_PATH + "ls_supply_officer.c")->move(this_object());
}
