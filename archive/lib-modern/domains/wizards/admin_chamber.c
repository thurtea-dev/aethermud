/* /domains/wizards/admin_chamber.c
   Admin-only chamber. Up from the lounge. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("The Admin Chamber");
    set_long(
        "A serious room with dark stone walls and minimal decoration. A long\n"
        "conference table seats eight. One wall holds a running log display of\n"
        "recent player activity and staff promotions. Sealed cabinets contain\n"
        "shutdown logs and policy records. Arch and head arch access only.\n"
    );
    set_exits( ([ "down" : "/domains/wizards/lounge" ]) );
}

void reset() {
    object bk;
    ::reset();
    if(!present("book", this_object())) {
        bk = clone_object("/domains/adm/wiz_tools/wiz_reference_book");
        if(bk) {
            bk->set_property("book_role", "admin");
            bk->move(this_object());
        }
    }
}
