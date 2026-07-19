/* /realms/thurtea/workroom.c
   Thurtea's personal workroom. */

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_properties( ([ "light" : 3, "indoors" : 1, "no attack" : 1 ]) );
    set_short("Thurtea's workroom");
    set_long(
        "A functional workspace. Shelves along the walls hold reference documents\n"
        "and notes. A large desk sits in the center. A staff supplies chest\n"
        "rests beside the desk with handbooks and wiz-tools inside.\n"
        "A request mailbox is mounted near the door.\n"
        "Type 'read mailbox' to review pending skill requests.\n"
        "Type 'look in chest' or 'take admin handbook from chest'.\n"
    );
    set_smell("default", "Ink, ozone, and cold coffee.");
    set_listen("default", "The occasional crackle of a ley line tapped through the walls.");
    set_exits( ([ "out" : "/domains/wizards/hallway" ]) );
}

void reset() {
    object mb;
    object hawk;
    object chest;
    int i;
    object *inv;

    ::reset();

    inv = all_inventory(this_object());

    mb = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(strsrch(base_name(inv[i]), "mailbox") != -1) {
            mb = inv[i];
            break;
        }
    }
    if(!mb) {
        mb = clone_object("/domains/adm/wiz_tools/mailbox");
        if(mb) mb->move(this_object());
    }

    hawk = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(strsrch(base_name(inv[i]), "thurtea_hawk") != -1) {
            hawk = inv[i];
            break;
        }
    }
    if(!hawk) {
        hawk = clone_object("/domains/Praxis/npcs/thurtea_hawk");
        if(hawk) hawk->move(this_object());
    }

    chest = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(base_name(inv[i]) == "/domains/adm/wiz_tools/staff_supplies_chest") {
            chest = inv[i];
            break;
        }
    }
    if(!chest) {
        chest = clone_object("/domains/adm/wiz_tools/staff_supplies_chest");
        if(chest) chest->move(this_object());
    }
}
