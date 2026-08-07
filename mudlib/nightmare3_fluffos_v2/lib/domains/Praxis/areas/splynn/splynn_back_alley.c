// /d/Praxis/areas/splynn/splynn_back_alley.c
// Back alley district south of the west road.
// Knock the pillar to reveal black market entrance.
// Down exit leads to catacomb entrance.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_short("Back Alley, Splynn");
    set_long(
        "A cramped alley running between high walls of shaped bone. The\n"
        "overhead panels give no light here; the only illumination comes\n"
        "from bioluminescent lichen growing in the gaps between stones.\n\n"
        "One wall has a heavy stone pillar standing free from the structure\n"
        "beside it. It does not appear to support anything. At the base\n"
        "of the alley, a iron grate set into the stone covers a dark shaft\n"
        "leading downward. The grate is unlocked.\n\n"
        "To knock: type 'knock pillar'.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/splynn_west_road",
        "east"  : "/domains/Praxis/areas/splynn/splynn_clan_hall",
        "down"  : "/domains/Praxis/areas/splynn/catacomb_entrance"
    ]) );
    set_listen("default",
        "Something dripping. Distant voices. From the grate below,\n"
        "a faint scraping sound and cold air.");
    set_smell("default",
        "Damp stone, lichen, and from the grate: cold earth and something old.");
    set_items( ([
        "pillar" : "A free-standing stone pillar about two meters tall. It has\n"
                    "no structural purpose. Someone placed it here deliberately.\n"
                    "There are three shallow grooves cut into its base.",
        "grate" : "Iron grate covering a shaft in the stone floor. Old construction.\n"
                    "The lock is missing. Cold air rises from the darkness below.",
        "lichen" : "Bioluminescent. Pale blue-green. The only light in the alley.",
        "walls" : "Shaped bone fitted with obsidian blocks. No windows.\n"
                    "No doors visible. Whatever is behind these walls is not for you.",
        "grooves" : "Three shallow grooves at the base of the pillar,\n"
                    "arranged like a handhold pattern. They look worn smooth from use."
    ]) );
}

int do_knock(string str) {
    if(!str || strsrch(lower_case(str), "pillar") == -1) {
        write("Knock what? The pillar is the only thing worth knocking here.\n");
        return 1;
    }
    if((int)this_object()->query_property("pillar_knocked")) {
        write("The passage is already open.\n");
        return 1;
    }
    this_object()->set_property("pillar_knocked", 1);
    this_object()->add_exit("west", "/domains/Praxis/areas/splynn/splynn_black_market");
    tell_room(this_object(),
        this_player()->query_cap_name() +
        " presses the three grooves in the base of the pillar in sequence.\n"
        "A section of the west wall shifts inward with a low grinding sound,\n"
        "revealing a narrow passage.\n",
        ({ }) );
    return 1;
}

void init() {
    ::init();
    add_action("do_knock", "knock");
}

void reset() {
    ::reset();
    if(!(int)query_property("pillar_knocked"))
        remove_exit("west");
}
