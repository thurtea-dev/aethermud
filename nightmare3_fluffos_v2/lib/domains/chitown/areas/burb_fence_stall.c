// /domains/chitown/areas/burb_fence_stall.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_short("Fence Stall");
    set_long(
        "A lean-to piled with unlabeled crates. Nothing on display looks\n"
        "legal for long. A chalkboard lists prices in credits and black\n"
        "credits without naming the goods.\n\n"
        "The night market is west. A narrow path north squeezes toward the\n"
        "maintenance tunnels under the wall district.");
    set_exits( ([
        "west"  : "/domains/chitown/areas/burb_night_market",
        "north" : "/domains/chitown/areas/chitown_maintenance_tunnel"
    ]) );
    set_listen("default",
        "Quiet. People here do not raise their voices.");
    set_smell("default",
        "Dusty wood and the metallic tang of old electronics.");
    set_items( ([
        "crates" : "No labels. No questions.",
        "chalkboard" : "Prices only. Descriptions are spoken, not written."
    ]) );
}

void reset() {
    ::reset();
    if(!present("contraband dealer", this_object()))
        clone_object("/domains/chitown/npcs/contraband_dealer")->move(this_object());
}
