/* /domains/Praxis/areas/splynn/alvurron/alvurron_spire.c
   Alvurron, Overlord's Spire. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, Overlord's Spire");
    set_long(
        "The base of a twisted spire of fused black stone at the heart of\n"
        "the inner district. The walls run with slow veins of red light,\n"
        "like something circulating. Kydian guards flank the stair that\n"
        "winds upward. The inner district is west.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/alvurron/alvurron_inner_district",
        "up" : "/domains/Praxis/areas/splynn/alvurron/alvurron_spire_top"
    ]) );
    set_listen("default",
        "A deep pulse in the stone, felt more than heard.");
    set_smell("default", "Hot mineral and ozone.");
    set_items( ([
        "veins" : "Red light moving slowly through the stone, always upward.",
        "stair" : "A wide spiral cut for wings as much as feet.",
        "spire" : "Fused, not built. The stone was persuaded into this shape."
    ]) );
}

void reset() {
    ::reset();
    if(!present("kydian overlord", this_object()))
        clone_object("/domains/Praxis/monsters/kydian_overlord")->move(this_object());
}
