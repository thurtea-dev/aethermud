/* /domains/NGR/areas/ngr_plaza.c
   NGR City, Civic Plaza. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("no attack", 1);
    set_short("NGR City, Civic Plaza");
    set_long(
        "The civic plaza of the border city, paved in interlocking concrete\n"
        "and swept spotless. A bronze statue of a Triax X-10 Predator power\n"
        "armor stands at the center on a plinth listing the honored dead of\n"
        "the gargoyle war. Every sight line is covered by a discreet gun\n"
        "emplacement. The market is south, a biergarten west, a residential\n"
        "block east, and the Triax showroom north.");
    set_exits( ([
        "south" : "/domains/NGR/areas/NGR_market_district",
        "west" : "/domains/NGR/areas/ngr_biergarten",
        "east" : "/domains/NGR/areas/ngr_residential",
        "north" : "/domains/NGR/areas/ngr_triax_showroom"
    ]) );
    set_listen("default",
        "Bootheels on concrete and a public address chime.");
    set_smell("default", "Concrete dust, diesel, and bakery bread.");
    set_items( ([
        "statue" : "A bronze X-10 Predator, twice life size. The plinth lists
names in small, dense columns.",
        "plinth" : "The honored dead of the gargoyle war. The list continues
on the other three faces.",
        "emplacements" : "Discreet, well maintained, and pointed at the sky."
    ]) );
}
