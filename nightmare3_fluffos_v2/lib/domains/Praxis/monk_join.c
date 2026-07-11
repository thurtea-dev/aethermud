#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no castle", 1);
    set_short("Monastery attic");
    set_long(
        "Dusty rafters and broken crates fill the monastery attic. Old monk\n"
        "robes hang on pegs, moth-eaten. Whatever order once trained here is\n"
        "gone. A sealed door to the east is boarded shut from this side.");
    set_items( ([
        "crates" : "Empty supply crates stamped with pre-Rifts Praxis markings.",
        "robes"  : "Faded brown robes. Useless now.",
        "door"   : "Boarded shut. Something collapsed beyond it long ago."
    ]) );
    set_exits( ([
        "down" : "/domains/Praxis/stairs"
    ]) );
}
