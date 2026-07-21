/* /domains/NewCamelot/areas/training_grounds.c
   Training Grounds. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Training Grounds");
    set_long(
        "A wide practice field of packed earth south of Market Street.\n"
        "Pells and archery butts line the far fence, and a rack of blunted\n"
        "practice weapons stands under a lean-to. Here the Dragon Knights\n"
        "and squires of New Camelot drill from dawn bell to dusk bell.\n"
        "Market Street is north.");
    set_listen("default",
        "The clack of practice blades and a drill sergeant's bark.");
    set_smell("default",
        "Dust, sweat, and oiled leather.");
    set_items( ([
        "pells" : "Oak posts gouged by ten thousand practice cuts.",
        "butts" : "Archery and energy-weapon targets, well ventilated.",
        "rack" : "Blunted swords and padded lances for drill work."
    ]) );
    set_exits( ([
        "north" : "/domains/NewCamelot/areas/market_street2"
    ]) );
}

void reset() {
    ::reset();
    if(!present("dragon knight", this_object()))
        new("/domains/NewCamelot/npcs/dragon_knight")->move(this_object());
    if(!present("squire", this_object()))
        new("/domains/NewCamelot/npcs/camelot_squire")->move(this_object());
}
