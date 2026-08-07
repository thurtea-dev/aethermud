/* /domains/Horton/areas/horton_rail_spur.c
   Overgrown rail spur east of the hovertrain station. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_short("Overgrown Rail Spur");
    set_long(
        "A dead mag-lev spur peels east from the station, rails buried under\n"
        "weeds and rust. Freight cars sit on their sides, stripped years ago.\n"
        "Kids from town dare each other this far. Adults do not.\n\n"
        "The station is west. A scrub path continues northeast.");
    set_exits( ([
        "west"      : "/domains/Horton/areas/horton_station",
        "northeast" : "/domains/Horton/areas/horton_scrub_ridge"
    ]) );
    set_listen("default", "Wind in dry weeds. A loose panel ticking.");
    set_smell("default", "Rust and crushed grass.");
    set_items( ([
        "rails" : "Mag-lev guide rails, cold and dead.",
        "cars"  : "Overturned freight shells. Nothing left inside but nests.",
        "weeds" : "Knee-high and sharp."
    ]) );
}

void reset() {
    ::reset();
    if(random(2) == 0 && !present("wild dog pack", this_object()))
        clone_object("/domains/Horton/monsters/wild_dog_pack")->move(this_object());
}
