/* /domains/wizards/lounge.c
   Wizard's Lounge - central hub for staff. Connected to the hallway north
   and to each position workroom plus the combat testing zone. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("The Wizard's Lounge");
    set_long(
        "A comfortable common area reserved for the wizard staff of RiftsMUD.\n"
        "Padded chairs and low tables are arranged around a central hearth.\n"
        "One wall holds pinned maps and domain-assignment sheets. A pedestal\n"
        "stands in one corner. A service counter runs along the far wall,\n"
        "fitted with a weapons bench, a blade rack, and a charging station.\n"
        "Halls lead to position workrooms and a combat testing zone.\n"
        "This area is restricted to wizard staff.\n"
    );
    set_exits( ([
        "north" : "/domains/wizards/hallway",
        "east"  : "/domains/wizards/rp_study",
        "west"  : "/domains/wizards/coding_workshop",
        "south" : "/domains/wizards/combat_test",
        "up"    : "/domains/wizards/admin_chamber",
        "down"  : "/domains/wizards/domain_office"
    ]) );
}

void reset() {
    object ob;
    ::reset();
    if(!present("rift opener", this_object())) {
        ob = clone_object("/domains/wizards/rift_opener");
        if(ob) ob->move(this_object());
    }
    if(!present("vasquez", this_object())) {
        ob = clone_object("/domains/wizards/lounge_rifle_vendor");
        if(ob) ob->move(this_object());
    }
    if(!present("brand", this_object())) {
        ob = clone_object("/domains/wizards/lounge_sword_vendor");
        if(ob) ob->move(this_object());
    }
    if(!present("zera", this_object())) {
        ob = clone_object("/domains/wizards/lounge_pet_trader");
        if(ob) ob->move(this_object());
    }
    if(!present("sparks", this_object())) {
        ob = clone_object("/domains/wizards/lounge_eclip_vendor");
        if(ob) ob->move(this_object());
    }
}
