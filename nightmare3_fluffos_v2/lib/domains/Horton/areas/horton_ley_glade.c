/* /domains/Horton/areas/horton_ley_glade.c
   Small ley-touched clearing east of the logging road. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("wilderness", 1);
    set_property("ley line", 1);
    set_short("Ley Glade");
    set_long(
        "A circular clearing where the grass grows too green. Stones in a\n"
        "rough ring hum faintly underfoot. Mages from farther towns would\n"
        "call this a minor ley node. Locals call it the humming place and\n"
        "leave offerings they do not discuss.\n\n"
        "Logging road west. Ancient grove north.");
    set_exits( ([
        "west"  : "/domains/Horton/areas/horton_logging_road",
        "north" : "/domains/Horton/areas/horton_ancient_grove"
    ]) );
    set_listen("default", "A low hum under the bird noise.");
    set_smell("default", "Ozone after rain, though the sky is clear.");
    set_items( ([
        "stones" : "Warm. Carved with marks that are not local.",
        "grass"  : "Unnaturally vivid. Soft.",
        "ring"   : "Seven stones. One cracked."
    ]) );
}

void reset() {
    ::reset();
    if(random(4) == 0 && !present("jungle stalker", this_object()))
        clone_object("/domains/Praxis/monsters/jungle_stalker")->move(this_object());
}
