/* /domains/wizards/domain_office.c
   Communal workspace for Domain Wizards. Down from the lounge. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Domain Wizard's Office");
    set_long(
        "A tidy office with a broad planning table at the center. Large maps\n"
        "of active game domains cover the walls, annotated with room counts\n"
        "and builder assignments. Filing cabinets hold domain access logs and\n"
        "assignment records. For Domain Wizard use.\n"
    );
    set_exits( ([ "up" : "/domains/wizards/lounge" ]) );
}

void reset() {
    object bk;
    ::reset();
    if(!present("book", this_object())) {
        bk = clone_object("/domains/adm/wiz_tools/wiz_reference_book");
        if(bk) {
            bk->set_property("book_role", "domain");
            bk->move(this_object());
        }
    }
}
