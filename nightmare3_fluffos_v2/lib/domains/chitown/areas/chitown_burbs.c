/* /domains/chitown/areas/chitown_burbs.c
   The Burbs outside Chi-Town. */

#include <std.h>
#include <rooms.h>
#include <daemons.h>

#define HOVERTRAIN "/domains/Horton/areas/hovertrain"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Chi-Town, The Burbs");
    set_long(
        "Ramshackle buildings crowd the streets outside the Coalition wall.\n"
        "Pre-Rifts materials, salvaged panels, and scavenged timber lean\n"
        "against each other in a precarious patchwork of desperation. The\n"
        "smells of cooking, waste, and open fires mix in the unfiltered air.\n\n"
        "The massive city wall looms to the north, its automated turrets\n"
        "visible above the roofline. A dark alley opens nearby. A rusted sewer\n"
        "grate opens into darkness beside the street. The Coalition clinic is\n"
        "east. A market operates to the west. Illinois Road lies south.\n"
        "Southeast, a Dog Boy checkpoint watches a side approach.\n"
        "A small hovertrain platform sits at the edge of the burbs,\n"
        "connecting to the town of Horton. Type 'board hovertrain' to board (50 credits).");
    set_exits( ([
        "north" : "/domains/chitown/areas/chitown_gate",
        "south" : "/domains/chitown/areas/illinois_road",
        "east"  : "/domains/chitown/areas/chitown_clinic",
        "west"  : "/domains/chitown/areas/chitown_market",
        "alley"  : "/domains/chitown/areas/chitown_alley",
        "sewer"  : "/domains/chitown/areas/chitown_sewer_entrance",
        "southeast" : "/domains/chitown/areas/burb_dogboy_checkpoint"
    ]) );
    set_listen("default",
        "Arguments, crying children, the distant thud of Coalition boots, "
        "and the crack of a barrel fire.");
    set_smell("default",
        "Cooking food, smoke, waste, and the chemical tang drifting over "
        "the wall from the city.");
    set_items( ([
        "wall" : "The immense Chi-Town ferrocrete wall. Turrets track everything.",
        "buildings" : "Patchwork structures built from whatever could be salvaged. "
                      "They lean on each other for support.",
        "fire" : "A barrel fire surrounded by Burbies trying to stay warm.",
        "crowd" : "Displaced humans, D-Bees, and everything in between. "
                      "All of them outside the wall.",
        "alley" : "A narrow passage disappearing into shadow."
    ]) );
}

void init() {
    ::init();
    add_action("cmd_board_train", "board");
    add_action("cmd_board_train", "hovertrain");
}

int cmd_board_train(string str) {
    object train;
    string lstr;

    lstr = str ? lower_case(str) : "";
    /* "board train", "board hovertrain", or just "hovertrain" (verb) */
    if(query_verb() == "hovertrain") {
        /* old verb kept for backward compat */
    } else if(lstr != "train" && lstr != "hovertrain") {
        notify_fail("Board what? Try 'board hovertrain'.\n");
        return 0;
    }
    train = find_object(HOVERTRAIN);
    if(!train)
        train = load_object(HOVERTRAIN);
    if(!train) {
        write("The hovertrain is not currently available.\n");
        return 1;
    }
    (int)train->board_player(this_player());
    return 1;
}

void reset() {
    ::reset();
    if(!find_object(HOVERTRAIN))
        catch(load_object(HOVERTRAIN));
}
