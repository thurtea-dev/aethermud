/* /domains/chitown/areas/chitown_black_market.c
   Hidden black market in a cleared junction off the maintenance tunnel. */

#include <std.h>
#include <rooms.h>

#define MOB_PATH "/domains/chitown/monsters/"
#define EQ_PATH "/domains/chitown/equipment/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Chi-Town Black Market");
    set_long(
        "A cleared junction serves as an improvised black market. Goods that\n"
        "would earn a Coalition firing squad sit in crates along the walls.\n"
        "The operator knows every face and forgets them just as fast.");
    set_exits( ([
        "north" : "/domains/chitown/areas/chitown_maintenance_tunnel"
    ]) );
    set_listen("default",
        "Quiet haggling and the clink of credits changing hands.");
    set_smell("default",
        "Packing oil, cardboard, and stale smoke.");
    set_items( ([
        "crates"   : "Crates of unmarked goods stacked along the walls.",
        "junction" : "A cleared tunnel junction, swept and lit better than the rest.",
        "operator" : "The fence. They know every face down here and forget them just as fast."
    ]) );
}

void reset() {
    ::reset();
    if(!present("chi-town fence", this_object()))
        clone_object(MOB_PATH + "chitown_fence")->move(this_object());
    if(!present("chitown contraband", this_object()))
        clone_object(EQ_PATH + "chitown_contraband")->move(this_object());
}
