// /domains/newcamelot/areas/camelot_armoury.c
// New Camelot armoury. Sells medieval and hybrid weapons.

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/newcamelot/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("New Camelot, Armory");
    set_long(
        "A well-stocked armory smelling of oil and fresh metal. Swords,\n"
        "shields, and armor hang on racks lining the walls. A forge glows\n"
        "at the back. The armorer, a broad woman with scarred forearms,\n"
        "looks up from a workbench covered in tools and half-finished pieces.\n"
        "\nType 'list' to see available goods. 'buy <item>' to purchase.");
    set_exits( ([ "west" : "/domains/newcamelot/areas/camelot_square" ]) );
    set_listen("default",
        "The ring of the forge, the rasp of a whetstone, and the armorer working.");
    set_smell("default",
        "Hot metal, oil, leather, and the dry smell of polished steel.");
    set_items( ([
        "racks" : "Wall racks holding swords, shields, and pieces of armor.",
        "forge" : "A working forge at the back. Still hot.",
        "workbench": "Covered in tools, metal pieces, and a half-finished gauntlet.",
        "armor" : "Several pieces of armor on display. Quality work."
    ]) );
}

void reset() {
    ::reset();
    if(!present("camelot armorer", this_object()))
        clone_object(NPC_PATH+"camelot_armourer")->move(this_object());
}
