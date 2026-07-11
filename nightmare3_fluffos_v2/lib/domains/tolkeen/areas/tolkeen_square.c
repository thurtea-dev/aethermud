// /domains/tolkeen/areas/tolkeen_square.c

#include <std.h>
#include <rooms.h>

#define MOXIM_PATH "/domains/Praxis/monsters/moxim"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_property("no attack", 1);
    set_short("Tolkeen, Liberation Square");
    set_long(
        "A broad plaza paved with salvaged ferrocrete and ward stones.\n"
        "Banners show the Tolkeen crest: a fist clutching a lightning bolt.\n"
        "Mercenaries, mages, and refugees mix in the crowd.\n\n"
        "The mage gate is south. Ward market east. Barracks west. Mage\n"
        "quarter northeast. Inn northwest. North wall walk ahead. A crypt\n"
        "stair descends beside the central monument.");
    set_exits( ([
        "south"     : "/domains/tolkeen/areas/tolkeen_gate",
        "east"      : "/domains/tolkeen/areas/tolkeen_market",
        "west"      : "/domains/tolkeen/areas/tolkeen_barracks",
        "north"     : "/domains/tolkeen/areas/tolkeen_wall_n",
        "northeast" : "/domains/tolkeen/areas/tolkeen_mage_quarter",
        "northwest" : "/domains/tolkeen/areas/tolkeen_inn",
        "down"      : "/domains/tolkeen/areas/tolkeen_crypt_entrance"
    ]) );
    set_listen("default", "Crowd noise, ward hum, a distant drill call.");
    set_smell("default", "Cookfires, ozone, and wet stone.");
    set_items( ([
        "banners" : "Lightning fist on black cloth. Burned CS posters at the edges.",
        "monument" : "A cracked statue of a ley line walker. Crypt stair beside it.",
        "ward stones" : "Set into the plaza. They warm underfoot."
    ]) );
}

void reset() {
    ::reset();
    if(!present("moxim", this_object()))
        clone_object(MOXIM_PATH)->move(this_object());
    if(!present("tolkeen militia", this_object()))
        clone_object("/domains/tolkeen/npcs/tolkeen_militia")->move(this_object());
}
