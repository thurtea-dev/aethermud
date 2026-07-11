/* /domains/Praxis/cleric_hall.c
   Gutted meeting house, former religious gathering space. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_properties( ([ "no attack":1, "light" : 2, "indoors" : 1]) );
    set_short("a gutted meeting house");
    set_long(
        "The interior walls are stripped bare, the floorboards warped and "
        "split where moisture got in. Rows of overturned benches lie half-"
        "buried under plaster rubble from a collapsed ceiling. Whatever "
        "group once gathered here cleared out long ago, taking everything "
        "of value with them. Only the smell of old wood and damp stone "
        "remains.\n"
    );
    set_exits(
        (["south" : "/domains/Praxis/cleric_join",
          "down" : "/domains/Praxis/crypt.c",
          "council" : "/domains/Praxis/council_hall",
          "east" : "/domains/Praxis/cleric_vote"]) );
}
