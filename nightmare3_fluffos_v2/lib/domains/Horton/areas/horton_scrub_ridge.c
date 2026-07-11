/* /domains/Horton/areas/horton_scrub_ridge.c
   Low ridge of scrub east of trail and rail spur. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Scrub Ridge");
    set_long(
        "A low ridge of thorn and scrub. From the crest you can see Horton's\n"
        "roofs and the dark forest north. Boot paths cut both ways - militia\n"
        "patrols and people who do not want to meet them.\n\n"
        "Wilderness trail west. Rail spur southwest. Bandit camp east.");
    set_exits( ([
        "west"      : "/domains/Horton/areas/horton_wilderness_trail",
        "southwest" : "/domains/Horton/areas/horton_rail_spur",
        "east"      : "/domains/Horton/areas/horton_bandit_camp"
    ]) );
    set_listen("default", "Wind on the ridge. Distant hovertrain hum.");
    set_smell("default", "Dry grass and gun oil on the breeze.");
    set_items( ([
        "ridge" : "Good sight lines. Bad cover if someone shoots first.",
        "paths" : "Two sets of habits sharing one landscape.",
        "roofs" : "Horton looks small from here. That is the point."
    ]) );
}

void reset() {
    ::reset();
    if(random(3) == 0 && !present("trail bandit", this_object()))
        clone_object("/domains/Horton/monsters/trail_bandit")->move(this_object());
}
