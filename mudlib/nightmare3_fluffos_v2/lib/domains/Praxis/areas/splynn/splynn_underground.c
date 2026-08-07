// /d/Praxis/areas/splynn/splynn_underground.c
// The buried chamber: reached by digging through the Splynn Market plaza.
// The EX-5 Behemoth sits here dormant.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("vehicle_accessible", 1);
    set_property("ex5_accessible", 1);
    set_short("Buried Chamber: The EX-5");
    set_long(
        "You are in a vast underground chamber carved by the collapse of a\n"
        "pre-Rifts substructure. Emergency lighting from cracked glow-strips\n"
        "casts pale blue light across the walls. The air is thick with dust\n"
        "and machine oil.\n\n"
        "And there it is.\n\n"
        "The EX-5 Behemoth fills most of the chamber. It stands dormant,\n"
        "one enormous fist resting against the far wall, cracked open at the\n"
        "hull access point on its lower torso. No lights. No sound. Just\n"
        "two hundred and forty metric tons of MDC-plated giant robot, waiting.\n\n"
        "A fuel pump conduit is bolted to the north wall. It connects to\n"
        "underground fuel reserves. The EX-5 can be refueled here.\n\n"
        "Type 'enter ex-5' or 'enter behemoth' to board the machine.\n"
        "Exit: up (the tunnel you fell through, which has collapsed and must be\n"
        "re-dug from above to use again).");
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/splynn_market"
    ]) );
    set_listen("default",
        "The drip of water somewhere deeper, the settling of old stone,\n"
        "and the faint tick of cooling metal from the EX-5.");
    set_smell("default",
        "Machine oil, stone dust, and the faint metallic smell of old MDC armor.");
    set_items( ([
        "ex-5" : "The EX-5 Behemoth. It is real. It is enormous. Type 'enter ex-5'\n"
                       "to board. Hull access is on the lower torso.",
        "behemoth" : "The EX-5 Behemoth. It is real. It is enormous. Type 'enter ex-5'\n"
                       "to board. Hull access is on the lower torso.",
        "fuel pump" : "A conduit pump bolted to the north wall. A thick hose snakes\n"
                       "from it toward the EX-5's fuel port. Type 'refuel ex-5' to use it.",
        "glow strips": "Emergency lighting strips, cracked and flickering but functional.\n"
                       "They have been here a very long time.",
        "chamber" : "The walls show the marks of a violent collapse: sheared stone,\n"
                       "bent rebar, crushed conduit. Whatever happened here was catastrophic.",
        "tunnel" : "The hole you fell through. It is above you. The edges have\n"
                       "collapsed inward. Someone would need to dig again from above."
    ]) );
}

void init() {
    ::init();
    add_action("cmd_enter_ex5", "enter");
}

int cmd_enter_ex5(string str) {
    object hull;

    if(!str) return 0;
    str = lower_case(str);
    if(strsrch(str, "ex-5") == -1 && strsrch(str, "ex5") == -1 &&
       strsrch(str, "behemoth") == -1) return 0;
    hull = load_object("/domains/Praxis/vehicles/ex5/ex5_hull");
    if(!hull) {
        write("The EX-5 cannot be entered right now.\n");
        return 1;
    }
    if(this_player()->move(hull)) {
        write("You cannot enter the EX-5.\n");
        return 1;
    }
    write("You climb through the hull access hatch into the EX-5 Behemoth.\n");
    tell_room(this_object(),
        this_player()->query_cap_name() + " climbs into the EX-5 Behemoth.\n",
        ({ this_player() }));
    this_player()->describe_current_room(1);
    return 1;
}

void reset() {
    ::reset();
}
