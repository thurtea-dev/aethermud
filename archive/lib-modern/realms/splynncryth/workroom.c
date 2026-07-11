/* /realms/splynncryth/workroom.c
   Personal workroom for Splynncryth. */
#include <std.h>
#include <rooms.h>
inherit ROOM;
void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Splynncryth's workroom");
    set_long(
        "A functional workspace. Shelves hold reference documents\n"
        "and notes. A large desk sits in the center.\n"
        "A request mailbox is mounted near the door.\n"
        "Type 'read mailbox' to review pending skill requests.\n"
    );
    set_exits( (["out" : "/domains/wizards/hallway"]) );
}
void reset() {
    object mb;
    object bk;
    ::reset();
    if(!present("mailbox", this_object())) {
        mb = clone_object("/domains/adm/wiz_tools/mailbox");
        if(mb) mb->move(this_object());
    }
    if(!present("book", this_object())) {
        bk = clone_object("/domains/adm/wiz_tools/wiz_reference_book");
        if(bk) {
            bk->set_property("book_role", "coding");
            bk->move(this_object());
        }
    }
}
