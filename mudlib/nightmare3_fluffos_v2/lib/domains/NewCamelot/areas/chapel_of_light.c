/* /domains/NewCamelot/areas/chapel_of_light.c
   Chapel of Light. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Chapel of Light");
    set_long(
        "A tall, narrow chapel walled in white stone and salvaged glass.\n"
        "Sunlight pours through the windows and pools on the floor in\n"
        "colored shapes. There are no idols here, only light: the chapel is\n"
        "dedicated to whatever good still finds its way through the Rifts.\n"
        "Market Street is west.");
    set_listen("default",
        "Quiet. The kind that settles the heart.");
    set_smell("default",
        "Incense and clean stone.");
    set_items( ([
        "windows" : "Each pane a different color, salvaged from a hundred ruined buildings across Europe.",
        "light" : "It fills the chapel like water fills a bowl.",
        "pews" : "Plain wooden benches, polished by use."
    ]) );
    set_exits( ([
        "west" : "/domains/NewCamelot/areas/market_street2"
    ]) );
}

void reset() {
    ::reset();
    if(!present("priestess", this_object()))
        new("/domains/NewCamelot/npcs/chapel_priestess")->move(this_object());
}
