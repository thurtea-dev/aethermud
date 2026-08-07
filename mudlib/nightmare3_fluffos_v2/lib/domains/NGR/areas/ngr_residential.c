/* /domains/NGR/areas/ngr_residential.c
   NGR City, Residential Block. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("no attack", 1);
    set_short("NGR City, Residential Block");
    set_long(
        "A tidy block of prefab housing, every unit identical and every\n"
        "windowbox different. Laundry lines run between roofs. Children\n"
        "play field-stripping drills with wooden rifles while a pensioner\n"
        "keeps score. The plaza is west.");
    set_exits( ([
        "west" : "/domains/NGR/areas/ngr_plaza"
    ]) );
    set_listen("default",
        "Children counting down a drill and laundry snapping in the wind.");
    set_smell("default", "Soap, geraniums, and gun oil.");
    set_items( ([
        "housing" : "Triax prefab units: ugly, indestructible, warm in winter.",
        "windowboxes" : "Geraniums, herbs, and in one case a very small
grapevine going places.",
        "children" : "Running field-strip drills on wooden rifles. Fastest one
gets first pick at dinner, apparently."
    ]) );
}

void reset() {
    ::reset();
    if(!present("civilian", this_object()))
        clone_object("/domains/NGR/npcs/ngr_civilian")->move(this_object());
}
