/* /domains/NGR/areas/ngr_biergarten.c
   NGR City, Biergarten. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("no attack", 1);
    set_short("NGR City, Biergarten");
    set_long(
        "A walled beer garden shaded by chestnut trees that survived the\n"
        "apocalypse better than most of Germany did. Long tables of scrubbed\n"
        "pine, steins of dark beer, and off-duty soldiers arguing football\n"
        "and firepower in equal measure. The plaza is east.");
    set_exits( ([
        "east" : "/domains/NGR/areas/ngr_plaza"
    ]) );
    set_listen("default",
        "Laughter, clinking steins, and a badly tuned accordion.");
    set_smell("default", "Dark beer, sausage, and chestnut leaves.");
    set_items( ([
        "trees" : "Chestnuts, old and stubborn. Shrapnel scars have healed
over in the bark.",
        "tables" : "Long pine tables. You could strip a rail gun on them, and
someone probably has.",
        "steins" : "Heavy ceramic, hard to break. This matters some nights."
    ]) );
}

void reset() {
    ::reset();
    if(!present("barkeep", this_object()))
        clone_object("/domains/NGR/npcs/ngr_barkeep")->move(this_object());
}
