// /d/Praxis/vehicles/ex5/ex5_crew_quarters.c
// EX-5 Behemoth -- crew quarters. Search lockers for random salvage on first visit.

#include <std.h>
#include <rooms.h>

#define EQUIP_PATH "/domains/Praxis/equipment/"

inherit ROOM;

private int __searched;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_property("vehicle_interior", 1);
    set_property("vehicle_daemon", "/daemon/ex5_d");
    __searched = 0;
    set_short("EX-5 Behemoth: Crew Quarters");
    set_long(
        "A spartan row of bunks welded to the port wall, each with a storage\n"
        "locker underneath. Military-grade, functional and nothing more.\n"
        "The mattresses have long since rotted away, leaving bare metal frames.\n"
        "Personal effects have been left behind by whoever crewed this machine\n"
        "last: a cracked photograph pinned to one bunk, a coffee mug bolted\n"
        "to a table, a set of playing cards fanned out mid-game.\n\n"
        "The storage lockers are still sealed. Type 'search lockers' to\n"
        "examine them.\n\n"
        "Exit: north back to the main hull.");
    set_exits( ([
        "north" : "/domains/Praxis/vehicles/ex5/ex5_hull"
    ]) );
    set_listen("default",
        "The creak of old metal, and the distant hum of the EX-5's dormant systems.");
    set_smell("default",
        "Stale air that has not moved in decades, dust, and faint traces\n"
        "of the people who lived here.");
    set_items( ([
        "bunks" : "Welded metal frames, no mattresses. Military spartan.",
        "lockers" : "Under each bunk, a storage locker with a press-release latch.\n"
                       "Type 'search lockers' to examine them.",
        "photograph" : "A cracked photo pinned to one bunk. Two people standing in\n"
                       "front of a building you do not recognize. They are smiling.",
        "mug" : "A coffee mug bolted to a wall-mounted table. WORLDS BEST PILOT\n"
                       "is printed on it. Pre-Rifts manufacture.",
        "cards" : "A set of playing cards. Whoever was playing left mid-game.",
        "effects" : "Small personal items left behind. Someone lived here."
    ]) );
}

void init() {
    ::init();
    add_action("cmd_search", "search");
}

int cmd_search(string str) {
    string *salvage, chosen;
    object item;

    if(!str || strsrch(lower_case(str), "locker") == -1) return 0;
    if(__searched) {
        write("The lockers have already been searched. Nothing left.\n");
        return 1;
    }
    __searched = 1;
    write("You work the press-release latches on each locker in turn.\n");
    write("Most are empty. But in the third locker from the left...\n");
    salvage = ({
        EQUIP_PATH + "e_clip.c",
        EQUIP_PATH + "rations.c",
        EQUIP_PATH + "rope.c",
        EQUIP_PATH + "first_aid_kit.c",
        EQUIP_PATH + "military_radio.c"
    });
    chosen = salvage[random(sizeof(salvage))];
    item = clone_object(chosen);
    if(item) {
        item->move(this_player());
        write("You find " + (string)item->query_short() + ".\n");
        say(this_player()->query_cap_name() +
            " searches the crew quarters lockers.\n", this_player());
    } else {
        write("You find nothing but dust.\n");
    }
    return 1;
}

void reset() {
    ::reset();
}
