/* /domains/wizards/coding_workshop.c
   Communal workspace for Coding Wizards. West of the lounge. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Coding Wizard's Workshop");
    set_long(
        "A cluttered workspace for the coders who build this world. Whiteboards\n"
        "cover two walls, filled with inheritance diagrams and exit-map sketches.\n"
        "Stacks of LPC reference sheets share shelf space with Palladium source\n"
        "books and half-finished object files. A large terminal desk dominates\n"
        "the center. For Coding Wizard use.\n"
    );
    set_exits( ([ "east" : "/domains/wizards/lounge" ]) );
}

void reset() {
    object bk;
    ::reset();
    if(!present("book", this_object())) {
        bk = clone_object("/domains/adm/wiz_tools/wiz_reference_book");
        if(bk) {
            bk->set_property("book_role", "coding");
            bk->move(this_object());
        }
    }
}
