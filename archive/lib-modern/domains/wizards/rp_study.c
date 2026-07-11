/* /domains/wizards/rp_study.c
   Communal workspace for RP Wizards. East of the lounge. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("RP Wizard's Study");
    set_long(
        "A welcoming room with warm lighting. Comfortable chairs face a low\n"
        "table stacked with player records and skill-request logs. A message\n"
        "slot is built into the wall near the door. Shelves hold Palladium RPG\n"
        "rulebooks and language reference binders. For RP Wizard use.\n"
    );
    set_exits( ([ "west" : "/domains/wizards/lounge" ]) );
}

void reset() {
    object bk;
    object mb;
    ::reset();
    if(!present("book", this_object())) {
        bk = clone_object("/domains/adm/wiz_tools/wiz_reference_book");
        if(bk) {
            bk->set_property("book_role", "rp");
            bk->move(this_object());
        }
    }
    if(!present("mailbox", this_object())) {
        mb = clone_object("/domains/adm/wiz_tools/mailbox");
        if(mb) mb->move(this_object());
    }
}
