// /domains/chitown/areas/burb_chop_shop.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Burb Chop Shop");
    set_long(
        "A corrugated shed packed with vehicle guts: engines on blocks,\n"
        "e-clips in a locked cage, and a half-stripped hovercycle on a lift\n"
        "made from scavenged hydraulic jacks. Oil stains map the floor.\n\n"
        "Gang corner is north. A side door east leads to a night market stall.");
    set_exits( ([
        "north" : "/domains/chitown/areas/burb_gang_corner",
        "east"  : "/domains/chitown/areas/burb_night_market"
    ]) );
    set_listen("default",
        "A wrench turning, a radio playing static-heavy music.");
    set_smell("default",
        "Motor oil, ozone, and hot metal.");
    set_items( ([
        "hovercycle" : "Not street-legal. Not Coalition-legal. Still almost running.",
        "cage" : "Wire mesh locked with a heavy padlock. Energy clips glint inside.",
        "engines" : "A mix of combustion relics and modern electric cores."
    ]) );
}

void reset() {
    ::reset();
    if(!present("burb mechanic", this_object()))
        clone_object("/domains/chitown/npcs/burb_mechanic")->move(this_object());
}
